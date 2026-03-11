#include "game.h"
#include "vec2.h"
#include "food.h"
#include "snake.h"
#include <ctime>
#include <cstdlib>


Vec2 Food::RandomizeFoodPos(const GameConfig &config, const Snake &snake)
{
    static bool seeded = false;
    if (!seeded)
    {
        srand(time(NULL));
        seeded = true;
    }

    Vec2 foodPos;
    do
    {
        // x is row (gridWidth), y is col (gridLength)
        int randomX = rand() % config.gridWidth;
        int randomY = rand() % config.gridLength;
        foodPos = {randomX, randomY};

        // Ensure food doesn't spawn on the snake
        bool onSnake = false;
        for (const auto& part : snake.GetBody())
        {
            if (part == foodPos) onSnake = true;
        }
        if (!onSnake) break;
        
    } while (true);

    return foodPos;

}
