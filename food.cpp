#include "game.h"
#include "vec2.h"

#include <ctime>
#include <cstdlib>


Vec2 RandomizeFoodPos(GameConfig &config)
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
    } while (foodPos == config.snakePos);

    return foodPos;

}
