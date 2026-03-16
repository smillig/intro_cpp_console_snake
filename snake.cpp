#include "snake.h"
#include "input_handler.h"

Snake::Snake(Vec2 startPos, int gridLength, int gridWidth)
    : lastInput('w'), gridLength(gridLength), gridWidth(gridWidth)
{
    body.push_back(startPos);
}

Vec2 Snake::GetPosition() const
{
    return body.front();
}

const std::deque<Vec2>& Snake::GetBody() const
{
    return body;
}

bool Snake::CheckOutOfBounds() const
{
    Vec2 head = body.front();
    if (head.x < 0 || head.x >= gridWidth)
    {
        return true;
    }
    if (head.y < 0 || head.y >= gridLength)
    {
        return true;
    }
    return false;
}

bool Snake::CheckSelfCollision() const
{
    // Start checking from the second segment (index 1)
    // because the head (index 0) naturally overlaps with itself.
    for (size_t i = 1; i < body.size(); ++i)
    {
        if (body[i] == body.front())
        {
            return true;
        }
    }
    return false;
}

void Snake::Grow()
{
    growing = true;
}

void Snake::Update()
{
    Vec2 newHead = body.front();

    // Move the snake based on the last input
    switch (lastInput)
    {
    case 'w':
        newHead = newHead + Vec2{-1, 0};
        break;
    case 's':
        newHead = newHead + Vec2{1, 0};
        break;
    case 'a':
        newHead = newHead + Vec2{0, -1};
        break;
    case 'd':
        newHead = newHead + Vec2{0, 1};
        break;
    }

    body.push_front(newHead);

    if (growing)
    {
        growing = false;
    }
    else
    {
        body.pop_back();
    }
}

void Snake::HandleInput(ProgState &state)
{
    if (kbhit_wrapper()) // check if a key has been pressed
    {
        char input = getch_wrapper(); // get the character
        if (input == 'w' || input == 'a' || input == 's' || input == 'd')
        {
            // Prevent snake from reversing on itself
            if ((input == 'w' && lastInput != 's') ||
                (input == 's' && lastInput != 'w') ||
                (input == 'a' && lastInput != 'd') ||
                (input == 'd' && lastInput != 'a'))
            {
                lastInput = input;
            }
        }
        else if (input == 27) // ESC key to return to main menu
        {
            state = ProgState::MainMenu;
        }
    }
}
