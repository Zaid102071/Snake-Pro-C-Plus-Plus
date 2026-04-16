#pragma once

#include <cstdint>

namespace snake {

constexpr std::uint32_t WINDOW_WIDTH  = 800;
constexpr std::uint32_t WINDOW_HEIGHT = 600;
constexpr std::uint32_t CELL_SIZE      = 20;
constexpr std::uint32_t GRID_COLS      = WINDOW_WIDTH  / CELL_SIZE;
constexpr std::uint32_t GRID_ROWS      = WINDOW_HEIGHT / CELL_SIZE;
constexpr float        BASE_SPEED      = 0.12f;
constexpr float        SPEED_INCREMENT  = 0.005f;
constexpr std::uint32_t MAX_SCORE       = 9999;

constexpr const char* WINDOW_TITLE = "Snake Pro";
constexpr const char* FONT_PATH    = "assets/fonts/consola.ttf";

}
