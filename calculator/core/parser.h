#ifndef PARSER_H
#define PARSER_H

#include "expression.h"
#include <memory>
#include <optional>
#include <string>

// ParseResult:
// A structure that holds the result of trying to parse a mathematical expression.
// It holds either a successful parse (the AST) or a failure (an error message string).
struct ParseResult
{
    // The root node of the parsed Abstract Syntax Tree. Exclusively owned by this result.
    std::unique_ptr<ASTNode> ast;
    
    // std::optional safely represents a value that might or might not exist.
    // If parsing fails, this contains the error string. If successful, it is empty (std::nullopt).
    std::optional<std::string> error;
};

// parseExpression:
// Takes a raw string input (e.g., "3 + 5 * (2 - 1)") and converts it into a structured AST.
// Returns a ParseResult containing either the root of the tree or an error explanation.
ParseResult parseExpression(const std::string& input);

#endif // PARSER_H