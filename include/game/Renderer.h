#pragma once

#include "game/Position.h"
#include <string>
#include <vector>

namespace snakepro {

class Renderer {
public:
    Renderer(int width, int height);

    void render(const std::vector<Position>& snake, const Position& food,
                int score, int highScore, int level, bool paused, bool gameOver);
    void renderMenu();
    void renderGameOver(int score, int highScore);
    void renderPause();
    void clear();

private:
    int width_;
    int height_;

    void positionCursor(int x, int y);
};

}
