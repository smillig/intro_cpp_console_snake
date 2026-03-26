#include <iostream>
#include <vector>
#include "grid.h"
#include "game.h"
#include "snake.h"


GameGrid::GameGrid(int gridLength, int gridWidth, char emptyCell)
    : gridLength(gridLength), gridWidth(gridWidth), gridSize(gridLength * gridWidth)
{
    this->grid.resize(gridSize, emptyCell);
}

std::vector<char> GameGrid::getGrid() const
{
    return grid;
}

void GameGrid::setGrid(int index, char value)
{
    grid[index] = value;
}

void GameGrid::emptyGrid()
{
    grid.assign(gridSize, '.');
}

int GameGrid::getGridSize() const
{
    return gridSize;
}

void GameGrid::DrawGrid(const GameConfig &config, const Snake &snake)
{
    emptyGrid();
    
    // Set the snake head position in the grid
    const auto& body = snake.GetBody();
    for(size_t i = 0; i < body.size(); ++i)
    {
        setGrid(GridIndex(body[i]), i == 0 ? config.snakeHead : config.snakeBody);
    }

    setGrid(GridIndex(config.foodPos), config.foodCell);
    
    // create grid on terminal
    // FIX: game flickers from output to each grid row sequntially, output entire grid in one go will help a lot.
    for (int i = 0; i < gridWidth; i++) // i represents the row
    {
        for(int j = 0; j < gridLength; j++) // j represents the column
        {
            // To access an element in a 1D vector as if it were a 2D grid,
            // use the formula: row * num_columns + column. Here it's i * length + j.
            std::cout << grid[i * gridLength + j];
        }
        std::cout << '\n';
    }
}

int GameGrid::GridIndex(Vec2 pos) // row = x , col = y
{
    return pos.x * gridLength + pos.y;
}
