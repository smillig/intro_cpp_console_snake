#include <fstream>

int LoadHighScore()
{
    std::ifstream file("highscore.txt");
    int highScore = 0;
    if (file.is_open())
    {
        file >> highScore;
        file.close();
    }
    return highScore;
}

void SaveHighScore(int score)
{
    std::ofstream file("highscore.txt");
    if (file.is_open())
    {
        file << score;
        file.close();
    }
}
