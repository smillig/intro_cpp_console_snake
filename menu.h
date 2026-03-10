#pragma once
#include "game.h"

enum class ProgState
{
    MainMenu,
    Game,
    HighScores,
    Quit

};

void ClearScreen();
ProgState EnterMainMenu(ProgState &state);
ProgState EnterGame(ProgState &state, GameConfig &config);
ProgState EnterHighScores(ProgState &state, GameConfig &config);
