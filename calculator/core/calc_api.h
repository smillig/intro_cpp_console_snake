#ifndef CALC_API_H
#define CALC_API_H

#include "expression.h"
#include <memory>
#include <string>
#include <optional>
#include <vector>

// EvalResult: Holds the numerical result of an evaluation or an error message if it failed.
struct EvalResult
{
    double value;
    std::optional<std::string> error;
};

// History: Manages a chronological list of successfully evaluated math expressions.
class History
{
public:
    explicit History(size_t capacity = 10);
    void push(const std::shared_ptr<ASTNode>& ast, const std::string& repr);
    std::vector<std::pair<std::string, std::shared_ptr<ASTNode>>> list() const;
    void persist(const std::string& path) const;
    void restore(const std::string& path);
private:
    size_t capacity_;
    std::vector<std::pair<std::string, std::shared_ptr<ASTNode>>> buf_;
};

// Core API Functions for the calculator interface
std::pair<std::unique_ptr<ASTNode>, std::optional<std::string>> parseExpressionAPI(const std::string& input);
EvalResult evaluateAST(const ASTNode& ast, double x = 0.0);

#endif // CALC_API_H