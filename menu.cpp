#include <iostream>
#include <thread>
#include <chrono>
#include <conio.h>
#include <string>
#include "menu.h"
#include "snake.h"
#include "score_io.h"
#include "game.h"

void ClearScreen()
{
    // ANSI escape code: \033[2J clears screen, \033[1;1H moves cursor to top-left
    std::cout << "\033[2J\033[1;1H";
}

ProgState EnterMainMenu(ProgState &state)
{
    while (state == ProgState::MainMenu)
    {
        std::cout << "\n      Welcome to SNAKEY CONSOLE!!\n";

        // -----Main Menu------
        std::cout << "\n======= Snakey Console Main Menu =======\n";
        std::cout << "\n   ========= 1. Start Game =========\n";
        std::cout << "\n    ======== 2. High Scores ========\n";
        std::cout << "\n      ====== 3. Quit  ====== \n";
        std::cout << "\nEnter your choice: ";

        std::string input_line;
        std::getline(std::cin, input_line);

        // Input sanitization: check for a single digit character.
        if (input_line.length() != 1 || !std::isdigit(static_cast<unsigned char>(input_line[0])))
        {
            std::cout << "Invalid input. Please enter a single digit." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            // clear screen and restart the loop
            ClearScreen();
            continue;
        }

        char choice = input_line[0];

        switch (choice)
        {
            case '1':
                // Start Game
                state = ProgState::Game;
                break;
            case '2':
                // High Scores
                state = ProgState::HighScores;
                break;
            case '3':
                // Quit
                state = ProgState::Quit;
                break;
            default:
                std::cout << "Invalid choice. Please enter 1, 2, or 3." << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                // clear screen
                ClearScreen();
                // restart the loop
                continue;
        }
    }
    return state;
}

ProgState EnterGame(ProgState &state, GameConfig &config)
{
    ClearScreen();
    bool gameOver = false;
    // Set start position and reset last direction
    Vec2 startPos = {config.gridLength / 2, config.gridWidth / 2};
    config.snakePos = startPos;
    config.lastInput = 'w'; // Reset to default starting direction

    while (state == ProgState::Game)
    {
        ClearScreen();
        config.grid.DrawGrid(config);
        HandlePlayerInput(config, state);
        UpdateSnake(config);

        gameOver = CheckOutOfBounds(config);
        config.score++;
        
        if (gameOver)
        {
            if (config.score > config.highScore)
            {
                config.highScore = config.score;
                SaveHighScore(config.highScore);
            }
            std::cout << "\n--- GAME OVER ---\n";
            std::cout << "\nHigh score: " << config.highScore << "!\n";
            std::cout << "Your score: " << config.score << "!\n";
            std::cout << "Press any key to return to the main menu." << std::endl;
            config.score = 0;
            _getch(); // Wait for any key press
            ClearScreen();
            state = ProgState::MainMenu;
            break; 
        }

        // Pause for 1 second
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    return state;
}

ProgState EnterHighScores(ProgState &state, GameConfig &config)
{
    ClearScreen();
    while (state == ProgState::HighScores)
    {
        std::cout << "\n\n======= Snakey Console High Score =======\n";
        std::cout << "    ------ High Score:" << config.highScore << " ------\n";
        _getch(); // Wait for any key press
        ClearScreen();
        state = ProgState::MainMenu;
    }
    return state;
}
