#pragma once

#include "core/Config.h"
#include "core/Types.h"
#include "core/Math.h"
#include "game/Snake.h"
#include "game/ScoreManager.h"
#include "engine/ParticleSystem.h"
#include "engine/Renderer.h"
#include "engine/Colors.h"
#include <SFML/Graphics.hpp>
#include <vector>

namespace snakepro {

class Game {
public:
    Game();

    void run();

private:
    void handleInput();
    void update();
    void render();
    void resetGame();
    float getInterval() const;
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

    void spawnEatEffect(sf::Vector2f pos);
    void spawnDeathEffect();

    sf::RenderWindow window_;
    sf::Font font_;
    Snake snake_;
    Food food_;
    ScoreManager scoreManager_;
    ParticleSystem particles_;

    int score_ = 0;
    int highScore_ = 0;
    State state_ = State::Menu;
    int gridW_ = Config::kGridCols;
    int gridH_ = Config::kGridRows;
    float frameTime_ = 0;
    float moveTimer_ = 0;
    float pulseTimer_ = 0;
    float shakeAmount_ = 0;
    bool isNewHighScore_ = false;
    int cellSize_ = Config::kCellSize;
    int boardX_ = 0;
    int boardY_ = 0;
};

}
