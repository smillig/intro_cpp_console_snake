#pragma once

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
