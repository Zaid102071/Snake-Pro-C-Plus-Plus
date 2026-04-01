#pragma once

namespace snakepro {

struct Config {
    static constexpr int kDefaultWidth = 40;
    static constexpr int kDefaultHeight = 20;
    static constexpr int kInitialSpeed = 150;
    static constexpr int kSpeedIncrement = 5;
    static constexpr int kMinSpeed = 50;
    static constexpr int kScorePerFood = 10;
    static constexpr int kHighScoreFileMax = 10;
    static constexpr char kSnakeHeadChar = '@';
    static constexpr char kSnakeBodyChar = 'o';
    static constexpr char kFoodChar = '*';
    static constexpr char kBorderChar = '#';
    static constexpr char kEmptyChar = ' ';
    static constexpr const char* kGameTitle = "SNAKE PRO";
    static constexpr const char* kHighScoreFile = "highscores.dat";
};

}
