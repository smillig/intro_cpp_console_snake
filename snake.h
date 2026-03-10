#pragma once
#include "game.h"
#include "menu.h"

void UpdateSnake(GameConfig &config);
void HandlePlayerInput(GameConfig &config, ProgState &state);
bool CheckOutOfBounds(GameConfig &config);
