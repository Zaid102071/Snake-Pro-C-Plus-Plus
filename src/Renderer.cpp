#include "game/Renderer.h"
#include <iostream>
#include <algorithm>
#include <iomanip>

#ifdef _WIN32
#include <windows.h>
#endif

namespace snakepro {

Renderer::Renderer(int width, int height)
    : width_(width), height_(height) {}

void Renderer::clear() {
#ifdef _WIN32
    COORD topLeft = {0, 0};
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    GetConsoleScreenBufferInfo(console, &screen);
    FillConsoleOutputCharacterA(console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    FillConsoleOutputAttribute(console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
                               screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    SetConsoleCursorPosition(console, topLeft);
#else
    std::cout << "\033[2J\033[H";
#endif
}

void Renderer::positionCursor(int x, int y) {
#ifdef _WIN32
    COORD coord = {static_cast<SHORT>(x), static_cast<SHORT>(y)};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
#else
    std::cout << "\033[" << (y + 1) << ";" << (x + 1) << "H";
#endif
}

void Renderer::render(const std::vector<Position>& snake, const Position& food,
                      int score, int highScore, int level, bool paused, bool gameOver) {
    clear();

    std::cout << std::string(width_ + 4, ' ') << "\n";
    std::cout << "  ";
    for (int i = 0; i < width_; ++i) std::cout << Config::kBorderChar;
    std::cout << "  \n";

    for (int y = 0; y < height_; ++y) {
        std::cout << Config::kBorderChar;
        for (int x = 0; x < width_; ++x) {
            Position pos(x, y);
            if (pos == food) {
                std::cout << "\033[33m" << Config::kFoodChar << "\033[0m";
            } else if (pos == snake.front()) {
                std::cout << "\033[32m" << Config::kSnakeHeadChar << "\033[0m";
            } else if (std::find(snake.begin() + 1, snake.end(), pos) != snake.end()) {
                std::cout << "\033[36m" << Config::kSnakeBodyChar << "\033[0m";
            } else {
                std::cout << Config::kEmptyChar;
            }
        }
        std::cout << Config::kBorderChar << "\n";
    }

    std::cout << "  ";
    for (int i = 0; i < width_; ++i) std::cout << Config::kBorderChar;
    std::cout << "  \n";

    std::cout << std::string(width_ + 4, ' ') << "\n";

    std::cout << "  SCORE: " << std::setw(5) << score
              << "  HIGH: " << std::setw(5) << highScore
              << "  LEVEL: " << level << "\n";

    if (paused) {
        std::cout << "\n  [PAUSED] Press P to resume\n";
    }

    if (gameOver) {
        std::cout << "\n  [GAME OVER] Press R to restart or Q to quit\n";
    }

    std::cout << std::flush;
}

void Renderer::renderMenu() {
    clear();

    std::cout << "\n";
    std::cout << "  ========================================\n";
    std::cout << "           " << "\033[32m" << Config::kGameTitle << "\033[0m" << "\n";
    std::cout << "  ========================================\n";
    std::cout << "\n";
    std::cout << "  CONTROLS:\n";
    std::cout << "    Arrow Keys / WASD - Move\n";
    std::cout << "    P                 - Pause\n";
    std::cout << "    Q / ESC           - Quit\n";
    std::cout << "\n";
    std::cout << "  Press any key to start...\n";
    std::cout << std::flush;
}

void Renderer::renderGameOver(int score, int highScore) {
    clear();

    std::cout << "\n";
    std::cout << "  ========================================\n";
    std::cout << "            GAME OVER\n";
    std::cout << "  ========================================\n";
    std::cout << "\n";
    std::cout << "  FINAL SCORE: " << score << "\n";
    std::cout << "  HIGH SCORE:  " << highScore << "\n";
    std::cout << "\n";
    std::cout << "  Press R to restart or Q to quit\n";
    std::cout << std::flush;
}

void Renderer::renderPause() {
    std::cout << "\n  *** PAUSED ***\n";
    std::cout << "  Press P to resume\n";
    std::cout << std::flush;
}

}
