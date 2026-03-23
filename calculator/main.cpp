#include "calc_api.h"
#include <iostream>
#include <string>
#include <memory>
#include <cmath>

int main()
{
    // Initialize the history buffer to hold up to 10 expressions.
    History hist(10);
    // Load any previously saved history from the text file on disk.
    hist.restore("history.txt");
    
    std::string response;
    
    // Main REPL (Read-Eval-Print Loop)
    while (true)
    {
        std::cout << "Enter expression (or 'quit'): ";
        
        // Read a full line of input from the user. 
        // If the stream breaks (e.g., EOF / Ctrl+D), exit the loop safely.
        if (!std::getline(std::cin, response)) break;
        
        if (response == "quit") break;
        if (response.empty()) continue;

        // Step 1: Parse the raw string input into an Abstract Syntax Tree (AST).
        auto [ast, perr] = parseExpressionAPI(response);
        if (perr)
        {
            std::cout << "Invalid Symbol: " << *perr << '\n';
            continue;
        }
        
        // Step 2: Store the successfully parsed expression in the history.
        // We convert the unique_ptr to a shared_ptr because both the history buffer
        // and the current evaluation step below need concurrent access to it.
        std::shared_ptr<ASTNode> sharedAst = std::shared_ptr<ASTNode>(std::move(ast));
        hist.push(sharedAst, response);

        // Step 3: Evaluate the mathematical expression to get a numerical result.
        auto eval = evaluateAST(*sharedAst, 0.0);
        if (eval.error)
        {
            std::cout << "Evaluation error: " << *eval.error << '\n';
            continue;
        }
        
        // Step 4: Pretty output
        // Avoid printing a trailing ".0" for whole numbers (e.g., print "5" instead of "5.0").
        double v = eval.value;
        if (std::fabs(v - std::round(v)) < 1e-12) // Check if the number is basically an integer
            std::cout << static_cast<long long>(std::llround(v)) << '\n';
        else
            std::cout << v << '\n';
    }

    // Save the session's history back to the text file before exiting the program.
    hist.persist("history.txt");
    return 0;
}
