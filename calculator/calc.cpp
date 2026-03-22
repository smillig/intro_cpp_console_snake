#include <iostream>
#include <regex>
#include <string>
#include <charconv>

/*
* Assessment 1 - The Calculator
// reads a user input through a "cin" one line and then outputs the result on the next line
// continues reading input and calculating till user inters "quit"
** Requirements:
    - only whole numbers needed
    - required operators: +-* and /
    - ignore all whitespace
    - invalid input results in "Invalid Symbol"
    - can use decimal numbers
    - can write several numbers and operators in the same line
    - personal: use regex
    Extras:
    - Can use parentheses
    - operators to be evaluated sequentially from left to right and have correct precedence
    - Don't display more decimal places than necessary (Pretty output)
    Extra**:
    - solves linear equasions (x+2) * 3 = 6
    - solves quadratic equations
*/

int main()
{   
    // https://en.cppreference.com/w/cpp/regex.html
    // convert to regex
    std::regex to_expression("(\\d+)\\s*([\\+\\-\\*\\/])\\s*(\\d+)");
    std::string response{};
    // store the matches:
    std::smatch groups;
    while (response != "quit")
    {
        // ask for a problem
        std::cout << "Please enter a math problem to be solved.\n";
        // store in response
        std::getline(std::cin, response);
        // check if we have a match
        if (response != "quit" && std::regex_search(response, groups, to_expression))
        {
            int solution{};
            char op = groups.str(2)[0]; // Extract the operator character
            switch (op)
            {
                case '+':
                    solution = std::stoi(groups.str(1)) + std::stoi(groups.str(3));
                    break;
                case '-':
                    solution = std::stoi(groups.str(1)) - std::stoi(groups.str(3));
                    break;
                case '*':
                    solution = std::stoi(groups.str(1)) * std::stoi(groups.str(3));
                    break;
                case '/':
                    solution = std::stoi(groups.str(1)) / std::stoi(groups.str(3));
                    break;
                default:
                    std::cout << "Operator Invalid, try again.";
                    break;
            }
            
            std::cout << solution << '\n';
            
        }
        else
        {
            std::cout << "Invalid Symbol.\n";
        }
    }
    return 0;
}