#pragma once
#include "menu.h"
#include "vec2.h"
#include <deque>

class Snake
{
public:
    // Constructor initializes the snake at a starting position with board dimensions
    Snake(Vec2 startPos, int gridLength, int gridWidth);

    void Update();
    void Grow();
    void HandleInput(ProgState &state);
    bool CheckOutOfBounds() const;
    bool CheckSelfCollision() const;

    Vec2 GetPosition() const;
    // Helper to get the whole body (for drawing and collision checks)
    const std::deque<Vec2>& GetBody() const;

private:
    // initialize to default int = { 0 }
    std::deque<Vec2> body;
    bool growing = false;
    char lastInput;
    // Store grid dimensions for bounds checking
    int gridLength;
    int gridWidth;
};
