#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <thread>
#include <chrono>
#include <conio.h>

// create struct for snake position
struct Vec2
{
    int x;
    int y;

    bool operator==(const Vec2& other) const
    {
        return x == other.x && y == other.y;
    }

    Vec2 operator+(const Vec2& other) const
    {
        return {x + other.x, y + other.y};
    }
};

class GameGrid
{
private:
    int gridSize;
    std::vector<char> grid;

public:
    GameGrid(int gridSize, char emptyCell)
        : gridSize(gridSize), grid(gridSize, emptyCell) 
        {
            this->gridSize = gridSize;
            grid.resize(gridSize, emptyCell);
        }

    std::vector<char> getGrid() const
    {
        return grid;
    }

    void setGrid(int index, char value)
    {
        grid[index] = value;
    }

    void emptyGrid()
    {
        grid.assign(gridSize, '.');
    }

    int getGridSize() const
    {
        return gridSize;
    }
};

// this will be the constructor once moved to grid.cpp
/*
GameGrid::GameGrid(int gridSize, char emptyCell)
{
    // or is this my constructor?
    this->gridSize = gridSize;
    grid.resize(gridSize, emptyCell);
}
*/

// create struct for gameplay config items
struct GameConfig
{
    int gridLength = 10;
    int gridWidth = 10;
    int snakeLength = 1;
    Vec2 snakePos = {0, 0};
    char lastInput = 'w';
    char emptyCell = '.';
    char wallCell = '#';
    char foodCell = 'o';
    char snakeHead = '@';
    char snakeBody = '*';
    GameGrid grid = GameGrid(gridLength * gridWidth, emptyCell);
};

int GridIndex(Vec2 pos, int gridLength) // row = x , col = y
{
    return pos.x * gridLength + pos.y;
}

void ClearScreen()
{
    // ANSI escape code: \033[2J clears screen, \033[1;1H moves cursor to top-left
    std::cout << "\033[2J\033[1;1H";
}

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

void DrawGrid(GameConfig &config)
{
    config.grid.emptyGrid();
    // Set the snake head position in the grid
    config.grid.setGrid(GridIndex(config.snakePos, config.gridLength), config.snakeHead);
    
    // create grid on terminal
    for (int i = 0; i < config.gridWidth; i++) // i represents the row
    {
        for(int j = 0; j < config.gridLength; j++) // j represents the column
        {
            // To access an element in a 1D vector as if it were a 2D grid,
            // use the formula: row * num_columns + column. Here it's i * length + j.
            std::cout << config.grid.getGrid()[i * config.gridLength + j];
        }
        std::cout << '\n';
    }
}

enum class ProgState
{
    MainMenu,
    Game,
    HighScores,
    Quit

};

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
                std::cout << "High scores... (not implemented)" << std::endl;
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
        DrawGrid(config);
        HandlePlayerInput(config, state);
        UpdateSnake(config);

        gameOver = CheckOutOfBounds(config);
        if (gameOver)
        {
            std::cout << "\n--- GAME OVER ---\n";
            std::cout << "Press any key to return to the main menu." << std::endl;
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

ProgState EnterHighScores(ProgState &state)
{
    ClearScreen();
    while (state == ProgState::HighScores)
    {
        std::cout << "No High Scores yet.\nPress any key to return to main menu.";
        std::cin.get();
        state = ProgState::MainMenu;
    }
    return state;
}


int main()
{
    
    // int bestScore = 0;

    enum ProgState runningState = ProgState::MainMenu;
    GameConfig config;

    while (runningState != ProgState::Quit)
    {
        switch (runningState)
        {
            case ProgState::MainMenu:
                runningState = EnterMainMenu(runningState);
                continue;
            case ProgState::Game:
                runningState = EnterGame(runningState, config);
                continue;
            case ProgState::HighScores:
                runningState = EnterHighScores(runningState);
                continue;
            default:
                std::cout << "Unknown state. Entering Main Menu." << std::endl;
                runningState = ProgState::MainMenu;
        }

    }
    return 0;
}
