#pragma once
#include "game.h"
#include "vec2.h"

// Forward declaration to avoid circular dependency with snake.h
class Snake;

class Food
{
    public:
        static Vec2 RandomizeFoodPos(const GameConfig &config, const Snake &snake);
};
