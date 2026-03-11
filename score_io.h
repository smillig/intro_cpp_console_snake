#pragma once
#include <fstream>

int LoadHighScore();
void SaveHighScore(int score);
// TODO: move this to a class
// TODO: create a list of last 10 best scores
// TODO: play with sorting of the values perhaps a vector of int and char<3> so scores are arcade AAA format
// TODO: add obfiscation to save file base64 encode maybe?
// TODO: optional, use regex to parse name input
