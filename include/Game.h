#pragma once

#include "Snake.h"
#include "ScoreManager.h"
#include "Config.h"
#include "Direction.h"
#include <raylib.h>
#include <string>

namespace snakepro {

class Game {
public:
    Game();
    ~Game();

    void run();

private:
    enum class State { Menu, Playing, Paused, GameOver };

    void handleInput();
    void update();
    void render();
    void resetGame();
    int getSpeed() const;
    int getLevel() const;

    void drawBackground();
    void drawBoard();
    void drawSnake();
    void drawFood();
    void drawHUD();
    void drawMenu();
    void drawPaused();
    void drawGameOver();
    void drawGrid();
    void drawParticles();

    void spawnParticles(int x, int y);

    struct Particle {
        float x, y;
        float vx, vy;
        float life;
        float maxLife;
        Color color;
    };

    Snake snake_;
    Food food_;
    ScoreManager scoreManager_;

    int score_;
    State state_;
    int width_;
    int height_;
    int cellSize_;
    float frameCounter_;
    std::vector<Particle> particles_;
};

}
