#pragma once
#include <vector>
#include "vec2.h"

// Forward declaration to avoid circular dependency
struct GameConfig;
class Snake;

class GameGrid
{
    private:
        // FIX: member private variables shoud be prefixed with m
        int gridLength;
        int gridWidth;
        int gridSize;
        std::vector<char> grid;

    public:
        GameGrid(int gridLength, int gridWidth, char emptyCell);
        std::vector<char> getGrid() const;
        void setGrid(int index, char value);
        int getGridLength() const;
        int getGridWidth() const;
        void emptyGrid();
        int getGridSize() const;
        void DrawGrid(const GameConfig &config, const Snake &snake);
        int GridIndex(Vec2 pos);
};
