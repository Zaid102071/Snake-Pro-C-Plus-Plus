#pragma once

#include <string>

namespace snakepro {

struct GameConfig {
    static constexpr int kScreenWidth = 900;
    static constexpr int kScreenHeight = 700;
    static constexpr int kGridSize = 20;
    static constexpr int kCellSize = 25;
    static constexpr int kInitialSpeed = 8;
    static constexpr int kMaxSpeed = 20;
    static constexpr int kScorePerFood = 10;
    static constexpr const char* kGameTitle = "SNAKE PRO";
    static constexpr const char* kHighScoreFile = "highscores.dat";

    static constexpr int kBoardOffsetX = (kScreenWidth - kGridSize * kCellSize) / 2;
    static constexpr int kBoardOffsetY = 80;
};

}
