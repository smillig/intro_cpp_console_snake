#include "parser.h"
#include <vector>
#include <stack>
#include <string>
#include <optional>
#include <charconv>
#include <cctype>

namespace Parser
{
    // Token Types: The different kinds of meaningful "words" in our math expression.
    enum TokenType { TOK_NUMBER, TOK_OP, TOK_LPAREN, TOK_RPAREN };

    // Token: Represents a single piece of the math expression after the initial reading phase.
    struct Token
    {
        TokenType type;
        std::string text;
        double number = 0.0; // Populated if type is TOK_NUMBER
        char op = 0;         // Populated if type is TOK_OP
    };

    // Determines the order of operations (PEMDAS/BODMAS). Higher numbers happen first.
    int precedence(char op)
    {
        switch (op)
        {
            case '+': case '-': return 1;
            case '*': case '/': return 2;
            case '^': return 3;
        }
        return 0;
    }

    // Exponents are right-associative (e.g., 2^3^2 is evaluated as 2^(3^2), not (2^3)^2).
    bool isRightAssociative(char op)
    {
        return op == '^';
    }

    // Tokenize: Converts a raw string (like "3 + 4") into an ordered list of Tokens.
    // This is the "Lexical Analysis" phase of parsing.
    std::vector<Token> tokenize(const std::string& s, std::optional<std::string>& err)
    {
        std::vector<Token> out;
        size_t i = 0, n = s.size();
        while (i < n)
        {
            char c = s[i];
            // 1. Skip whitespace
            if (std::isspace((unsigned char)c)) { ++i; continue; }
            
            // 2. Extract numbers (including decimals)
            if ((c >= '0' && c <= '9') || c == '.')
            {
                size_t start = i;
                while (i < n && (std::isdigit((unsigned char)s[i]) || s[i] == '.')) ++i;
                std::string tok = s.substr(start, i - start);
                Token t{};
                t.type = TOK_NUMBER;
                t.text = tok;
                // std::from_chars is a very fast, modern C++ way to convert text to numbers
                auto [ptr, ec] = std::from_chars(tok.data(), tok.data() + tok.size(), t.number);
                if (ec != std::errc()) { err = "Invalid number: " + tok; return {}; }
                out.push_back(std::move(t));
                continue;
            }
            
            // 3. Extract operators
            if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^')
            {
                Token t{};
                t.type = TOK_OP;
                t.op = c;
                t.text = std::string(1, c);
                out.push_back(std::move(t));
                ++i;
                continue;
            }
            
            // 4. Extract Parentheses
            if (c == '(')
            {
                out.push_back(Token{TOK_LPAREN, "(", 0.0, 0});
                ++i;
                continue;
            }
            if (c == ')')
            {
                out.push_back(Token{TOK_RPAREN, ")", 0.0, 0});
                ++i;
                continue;
            }
            
            // 5. Unrecognized character fallback
            err = std::string("Invalid symbol: ") + c;
            return {};
        }
        return out;
    }

    // Shunting-Yard to AST builder:
    // This implements a modified version of Edsger Dijkstra's Shunting-Yard algorithm.
    // Instead of producing a reverse-polish notation (RPN) string, it dynamically builds
    // our AST tree (Abstract Syntax Tree) using a stack of operators and a stack of nodes.
    ParseResult shuntingYardToAST(const std::vector<Token>& tokens)
    {
        std::stack<char> ops;                          // Holds operators (+, *, etc) temporarily
        std::stack<std::unique_ptr<ASTNode>> vals;     // Holds the growing AST nodes
        
        // Lambda function: Pops one operator and two values, combines them into a single 
        // BinaryOpNode, and pushes that combined branch back onto the values stack.
        auto applyOp = [&]() -> bool {
            if (ops.empty()) return false;
            char op = ops.top(); ops.pop();
            if (vals.size() < 2) return false;
            auto right = std::move(vals.top()); vals.pop();
            auto left = std::move(vals.top()); vals.pop();
            vals.push(std::make_unique<BinaryOpNode>(op, std::move(left), std::move(right)));
            return true;
        };

        for (size_t i = 0; i < tokens.size(); ++i)
        {
            const Token& t = tokens[i];
            
            // If it's a number, just create a leaf node and push it to the values stack.
            if (t.type == TOK_NUMBER)
            {
                vals.push(std::make_unique<NumberNode>(t.number));
                continue;
            }
            
            // If it's an operator, apply higher-priority operators already on the stack first.
            if (t.type == TOK_OP)
            {
                char op = t.op;
                while (!ops.empty())
                {
                    char top = ops.top();
                    if (top == '(') break;
                    int p1 = precedence(top);
                    int p2 = precedence(op);
                    // Enforce Order of Operations and Associativity rules
                    if ((isRightAssociative(op) && p2 < p1) || (!isRightAssociative(op) && p2 <= p1))
                    {
                        if (!applyOp()) return ParseResult{nullptr, "Malformed expression (applyOp failed)."};
                    }
                    else break;
                }
                ops.push(op);
                continue;
            }
            
            // Left parens go straight onto the operator stack.
            if (t.type == TOK_LPAREN) { ops.push('('); continue; }
            
            // Right parens force evaluation of everything until we hit the matching left paren.
            if (t.type == TOK_RPAREN)
            {
                bool found = false;
                while (!ops.empty())
                {
                    char top = ops.top(); ops.pop();
                    if (top == '(') { found = true; break; }
                    // apply top
                    if (vals.size() < 2) return ParseResult{nullptr, "Malformed expression in parentheses."};
                    auto right = std::move(vals.top()); vals.pop();
                    auto left = std::move(vals.top()); vals.pop();
                    vals.push(std::make_unique<BinaryOpNode>(top, std::move(left), std::move(right)));
                }
                if (!found) return ParseResult{nullptr, "Mismatched parentheses."};
                continue;
            }
        }

        // Once we run out of tokens, apply whatever operators are left on the stack.
        while (!ops.empty())
        {
            char op = ops.top(); ops.pop();
            if (op == '(' || op == ')') return ParseResult{nullptr, "Mismatched parentheses."};
            if (vals.size() < 2) return ParseResult{nullptr, "Malformed expression at final reduce."};
            auto right = std::move(vals.top()); vals.pop();
            auto left = std::move(vals.top()); vals.pop();
            vals.push(std::make_unique<BinaryOpNode>(op, std::move(left), std::move(right)));
        }

        // At the very end, we should have exactly one unified AST node left: the root.
        if (vals.size() != 1) return ParseResult{nullptr, "Malformed expression; leftover values."};
        return ParseResult{std::move(vals.top()), std::nullopt};
    }
} // namespace

// The main entry point: combines tokenization and AST parsing into one easy function call.
ParseResult parseExpression(const std::string& input)
{
    std::optional<std::string> err;
    auto tokens = Parser::tokenize(input, err);
    if (err) return ParseResult{nullptr, err}; // Return early if tokenization failed
    return Parser::shuntingYardToAST(tokens);
}