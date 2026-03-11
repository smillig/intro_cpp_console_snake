#pragma once
#include "vec2.h"
#include "grid.h"

// create struct for gameplay config items
struct GameConfig
{
    int gridLength = 10;
    int gridWidth = 10;
    int score = 0;
    int highScore = 0;
    Vec2 foodPos = {0, 0};
    char emptyCell = '.';
    char wallCell = '#';
    char foodCell = 'o';
    char snakeHead = '@';
    char snakeBody = '*';
    GameGrid grid{gridLength, gridWidth, emptyCell};
};
