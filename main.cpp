#include <iostream>
#include "grid.h"
#include "game.h"
#include "vec2.h"
#include "score_io.h"
#include "menu.h"
#include "input_handler.h"


int main()
{
    InputManager inputManager; // Manages terminal state for cross-platform input
    enum ProgState runningState = ProgState::MainMenu;
    GameConfig config;
    config.highScore = LoadHighScore();

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
                runningState = EnterHighScores(runningState, config);
                continue;
            default:
                std::cout << "Unknown state. Entering Main Menu." << std::endl;
                runningState = ProgState::MainMenu;
        }

    }
    return 0;
}
