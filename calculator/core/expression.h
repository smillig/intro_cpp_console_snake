#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <iostream>
#include <memory>
#include <string>

// EvalContext (Evaluation Context):
// This is a simple struct used to pass the current "state" or "environment" down the 
// tree when evaluating the math expression. Right now, it just holds the value for 'x'. 
// Passing a context instead of using global variables makes the evaluation thread-safe 
// and allows you to easily expand to support multiple variables (e.g., y, z, time).
struct EvalContext
{
    double x = 0.0; // for future variable support
};

// ASTNode (Abstract Syntax Tree Node):
// The base class representing any piece of our parsed math expression.
// Because we don't know at compile time if a node is a Number or an Operation,
// we use an abstract base class with virtual functions (Polymorphism).
struct ASTNode
{
    // A virtual destructor is critical for base classes. It ensures that when a derived
    // object (like NumberNode) is deleted via a base class pointer (ASTNode*), the 
    // derived class's destructor is called properly, preventing memory leaks.
    virtual ~ASTNode() = default;
    
    // Recursively evaluates this part of the tree and returns the resulting number.
    virtual double evaluate(const EvalContext& ctx) const = 0;
    
    // Creates a "deep copy" of this node and all of its descendants.
    virtual std::unique_ptr<ASTNode> clone() const = 0;
    
    // Converts the tree back into a human-readable string form.
    virtual std::string toString() const = 0;
};

// NumberNode:
// Represents a basic number in the AST. These form the "leaves" of our tree structure.
struct NumberNode : ASTNode
{
    double value;
    explicit NumberNode(double v) : value(v) {}
    double evaluate(const EvalContext&) const override { return value; }
    std::unique_ptr<ASTNode> clone() const override { return std::make_unique<NumberNode>(value); }
    std::string toString() const override;
};

// BinaryOpNode (Binary Operator Node):
// Represents an operation that takes two arguments (like +, -, *, /). 
// This is an "internal" branch node in the tree that holds an operator and two children.
struct BinaryOpNode : ASTNode
{
    char op;
    
    // We use std::unique_ptr to manage the memory of the left and right child nodes.
    // When this BinaryOpNode is destroyed, its unique_ptrs automatically clean up their
    // respective child nodes. This forms a cascade of cleanups, preventing memory leaks.
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;
    
    BinaryOpNode(char o, std::unique_ptr<ASTNode> l, std::unique_ptr<ASTNode> r)
    // std::move:
    // Since unique_ptr enforces *exclusive* ownership, it cannot be copied. 
    // std::move explicitly transfers ownership of the dynamically allocated memory 
    // from the parameters 'l' and 'r' into the class members 'left' and 'right'.
    // After the move, the 'l' and 'r' parameters become null/empty.
    : op(o), left(std::move(l)), right(std::move(r)) {}
    
    double evaluate(const EvalContext& ctx) const override;
    std::unique_ptr<ASTNode> clone() const override;
    std::string toString() const override;
};
#endif // EXPRESSION_H