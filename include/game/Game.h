#pragma once

#include "core/Types.h"
#include "core/Settings.h"
#include "core/Math.h"
#include "game/Snake.h"
#include "game/ScoreManager.h"
#include "engine/ParticleSystem.h"
#include "engine/Renderer.h"
#include "engine/ColorPalette.h"
#include <raylib.h>
#include <vector>

namespace snakepro {

class Game {
public:
    Game();
    ~Game();

    void run();

private:
    void handleInput();
    void update();
    void render();
    void resetGame();
    int getSpeed() const;
    int getLevel() const;

    void drawBackground();
    void drawBoard();
    void drawGrid();
    void drawSnake();
    void drawFood();
    void drawHUD();
    void drawMenu();
    void drawPaused();
    void drawGameOver();
    void drawHighScores();

    void spawnEatEffect(Vec2 pos);
    void spawnDeathEffect();

    Snake snake_;
    Food food_;
    ScoreManager scoreManager_;
    Particle particles_;

    int score_ = 0;
    int highScore_ = 0;
    GameState state_ = GameState::Menu;
    int gridW_ = Settings::kGridCols;
    int gridH_ = Settings::kGridRows;
    float frameCounter_ = 0;
    float moveTimer_ = 0;
    float menuPulse_ = 0;
    float foodPulse_ = 0;
    bool isNewHighScore_ = false;
    int cellSize_ = 28;
    int boardX_ = 0;
    int boardY_ = 0;
};

}
