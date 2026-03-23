#include "expression.h"
#include <sstream>
#include <cmath>

// Converts the numeric value held by this node into a string.
std::string NumberNode::toString() const
{
    // std::ostringstream works just like std::cout, but instead of printing to the 
    // console, it writes the formatted output into an internal string buffer.
    std::ostringstream ss;
    ss << value;
    return ss.str(); // Extracts the built string from the stream.
}

// Evaluates the mathematical operation this node represents.
double BinaryOpNode::evaluate(const EvalContext& ctx) const
{
    // left and right are std::unique_ptr<ASTNode>s. 
    // We use the arrow operator '->' to access methods of the object the pointer points to.
    // This recursively evaluates the left and right sides of the tree down to the leaves (NumberNodes).
    double a = left->evaluate(ctx);
    double b = right->evaluate(ctx);
    
    // Perform the actual math operation based on the 'op' character.
    switch (op)
    {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b; // No divide-by-zero guard here; caller may handle
        // std::pow is from the <cmath> library and handles exponents (e.g., a^b)
        case '^': return std::pow(a, b);
        default: return 0.0;
    }
}

// Creates a complete, independent "deep copy" of this node and all its children.
std::unique_ptr<ASTNode> BinaryOpNode::clone() const
{
    // std::make_unique safely allocates memory for a new BinaryOpNode on the heap.
    // Notice that we call left->clone() and right->clone(). This guarantees that 
    // we don't just copy the pointers (which unique_ptr forbids anyway), but instead we 
    // recursively copy the entire sub-tree, maintaining exclusive ownership of the new nodes.
    return std::make_unique<BinaryOpNode>(op, left->clone(), right->clone());
}

// Converts the operation back into a human-readable text format.
std::string BinaryOpNode::toString() const
{
    // Recursively converts the left side, the operator, and the right side into strings,
    // and wraps the whole thing in parentheses to explicitly show the order of operations
    // (precedence) as parsed by the tree. 
    // e.g., ( ( 2 + 3 ) * x )
    return "(" + left->toString() + " " + std::string(1, op) + " " + right->toString() + ")";
}