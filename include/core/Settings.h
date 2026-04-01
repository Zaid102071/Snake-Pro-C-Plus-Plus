#pragma once

namespace snakepro {

struct Settings {
    static constexpr int kScreenWidth = 960;
    static constexpr int kScreenHeight = 720;
    static constexpr int kGridCols = 24;
    static constexpr int kGridRows = 18;
    static constexpr int kInitialSpeed = 6;
    static constexpr int kMaxSpeed = 18;
    static constexpr int kScorePerFood = 10;
    static constexpr const char* kGameTitle = "SNAKE PRO";
    static constexpr const char* kHighScoreFile = "highscores.dat";
    static constexpr const char* kVersion = "1.0.0";
};

}
