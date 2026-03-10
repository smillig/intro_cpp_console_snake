#include "game.h"
#include <conio.h>
#include "menu.h"

// returns true if in bounds else player has hit wall
bool CheckOutOfBounds(GameConfig &config)
{
    if (config.snakePos.x < 0 || config.snakePos.x >= config.gridWidth)
    {
        return true;
    }
    if (config.snakePos.y < 0 || config.snakePos.y >= config.gridLength)
    {
        return true;
    }
    return false;
}

void UpdateSnake(GameConfig &config)
{
    // Move the snake based on the last input
    switch (config.lastInput)
    {
    case 'w':
        config.snakePos = config.snakePos + Vec2{-1, 0};
        break;
    case 's':
        config.snakePos = config.snakePos + Vec2{1, 0};
        break;
    case 'a':
        config.snakePos = config.snakePos + Vec2{0, -1};
        break;
    case 'd':
        config.snakePos = config.snakePos + Vec2{0, 1};
        break;
    }
}

void HandlePlayerInput(GameConfig &config, ProgState &state)
{
    if (_kbhit()) // check if a key has been pressed
    {
        char input = _getch(); // get the character
        if (input == 'w' || input == 'a' || input == 's' || input == 'd')
        {
            // Prevent snake from reversing on itself
            if ((input == 'w' && config.lastInput != 's') ||
                (input == 's' && config.lastInput != 'w') ||
                (input == 'a' && config.lastInput != 'd') ||
                (input == 'd' && config.lastInput != 'a'))
            {
                config.lastInput = input;
            }
        }
        else if (input == 27) // ESC key to return to main menu
        {
            state = ProgState::MainMenu;
        }
    }
}
