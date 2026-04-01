#pragma once

namespace snakepro {

struct Config {
    static constexpr int kScreenWidth = 1000;
    static constexpr int kScreenHeight = 750;
    static constexpr int kGridCols = 28;
    static constexpr int kGridRows = 20;
    static constexpr int kCellSize = 28;
    static constexpr float kInitialInterval = 0.15f;
    static constexpr float kMinInterval = 0.055f;
    static constexpr int kScorePerFood = 10;
    static constexpr const char* kTitle = "SNAKE PRO";
    static constexpr const char* kVersion = "2.0.0";
};

}
