#include "calc_api.h"
#include "parser.h"
#include <fstream>

// parseExpressionAPI: A wrapper around the core parser.
// Returns a pair containing either the successfully parsed AST, or an error string.
std::pair<std::unique_ptr<ASTNode>, std::optional<std::string>> parseExpressionAPI(const std::string& input)
{
    auto res = parseExpression(input);
    if (res.error) return {nullptr, res.error};
    return {std::move(res.ast), std::nullopt};
}

// evaluateAST: Evaluates an AST tree with an optional 'x' variable context.
EvalResult evaluateAST(const ASTNode& ast, double x)
{
    EvalContext ctx; 
    ctx.x = x;
    // Simple runtime guard for division by zero could be implemented by checking denominator before division.
    double v = ast.evaluate(ctx);
    return EvalResult{v, std::nullopt};
}

/* History implementation */

// Constructor sets the maximum number of items the history can hold.
History::History(size_t capacity) : capacity_(capacity) {}

// push: Adds a new parsed expression and its text representation to the history.
void History::push(const std::shared_ptr<ASTNode>& ast, const std::string& repr)
{
    if (!ast) return;
    // If we've reached our maximum capacity, remove the oldest entry (the first one).
    if (buf_.size() == capacity_) buf_.erase(buf_.begin());
    buf_.push_back({repr, ast});
}

// list: Returns a copy of the history buffer.
std::vector<std::pair<std::string, std::shared_ptr<ASTNode>>> History::list() const
{
    return buf_;
}

// persist: Saves the text representations of the history items to a file.
void History::persist(const std::string& path) const
{
    // ios::trunc overwrites the file completely if it already exists.
    std::ofstream ofs(path, std::ios::trunc);
    if (!ofs) return;
    for (const auto& [repr, ast] : buf_)
    {
        ofs << repr << '\n';
    }
}

// restore: Loads previously saved text representations from a file and parses them.
void History::restore(const std::string& path)
{
    std::ifstream ifs(path);
    if (!ifs) return;
    buf_.clear();
    std::string line;
    while (std::getline(ifs, line))
    {
        if (line.empty()) continue;
        auto pr = parseExpression(line); // Reparse the line from the text file
        if (!pr.error && pr.ast)
        {
            // Convert the unique_ptr from parseExpression into a shared_ptr for the history buffer
            buf_.push_back({line, std::shared_ptr<ASTNode>(std::move(pr.ast))});
            if (buf_.size() >= capacity_) break; // Stop loading if we hit the limit
        }
    }
}