#include "Game.h"
#include <cmath>
#include <algorithm>
#include <sstream>

namespace snakepro {

Game::Game()
    : snake_(GameConfig::kGridSize / 2, GameConfig::kGridSize / 2),
      score_(0),
      state_(State::Menu),
      width_(GameConfig::kGridSize),
      height_(GameConfig::kGridSize),
      cellSize_(GameConfig::kCellSize),
      frameCounter_(0) {
    food_.spawn(width_, height_, snake_.getBody());
}

Game::~Game() {
    CloseWindow();
}

void Game::run() {
    InitWindow(GameConfig::kScreenWidth, GameConfig::kScreenHeight, GameConfig::kGameTitle);
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        handleInput();
        if (state_ == State::Playing) {
            update();
        }
        frameCounter_++;
        render();
    }
}

void Game::handleInput() {
    if (state_ == State::Menu) {
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
            state_ = State::Playing;
        }
        return;
    }

    if (state_ == State::GameOver) {
        if (IsKeyPressed(KEY_R)) {
            resetGame();
        } else if (IsKeyPressed(KEY_Q) || IsKeyPressed(KEY_ESCAPE)) {
            CloseWindow();
        }
        return;
    }

    if (state_ == State::Paused) {
        if (IsKeyPressed(KEY_P) || IsKeyPressed(KEY_ESCAPE)) {
            state_ = State::Playing;
        } else if (IsKeyPressed(KEY_Q)) {
            CloseWindow();
        }
        return;
    }

    if (IsKeyPressed(KEY_P) || IsKeyPressed(KEY_ESCAPE)) {
        state_ = State::Paused;
        return;
    }

    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        snake_.setDirection(Direction::Up);
    } else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
        snake_.setDirection(Direction::Down);
    } else if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
        snake_.setDirection(Direction::Left);
    } else if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
        snake_.setDirection(Direction::Right);
    }
}

void Game::update() {
    static float moveTimer = 0;
    float frameTime = GetFrameTime();
    moveTimer += frameTime;

    float moveInterval = 1.0f / static_cast<float>(getSpeed());

    if (moveTimer >= moveInterval) {
        moveTimer = 0;
        snake_.move();

        if (snake_.getHead() == food_.getPosition()) {
            score_ += GameConfig::kScorePerFood;
            spawnParticles(food_.getPosition().x, food_.getPosition().y);
            snake_.grow();
            food_.spawn(width_, height_, snake_.getBody());
        }

        if (snake_.checkSelfCollision() || snake_.checkWallCollision(width_, height_)) {
            state_ = State::GameOver;
            scoreManager_.addScore(score_);
        }
    }

    for (auto it = particles_.begin(); it != particles_.end();) {
        it->x += it->vx * frameTime;
        it->y += it->vy * frameTime;
        it->life -= frameTime;
        if (it->life <= 0) {
            it = particles_.erase(it);
        } else {
            ++it;
        }
    }
}

void Game::render() {
    BeginDrawing();
    ClearBackground(Color{15, 15, 25, 255});

    drawBackground();
    drawBoard();

    if (state_ == State::Menu) {
        drawMenu();
    } else {
        drawGrid();
        drawSnake();
        drawFood();
        drawHUD();
        drawParticles();

        if (state_ == State::Paused) drawPaused();
        if (state_ == State::GameOver) drawGameOver();
    }

    EndDrawing();
}

void Game::drawBackground() {
    for (int i = 0; i < GameConfig::kScreenWidth; i += 40) {
        for (int j = 0; j < GameConfig::kScreenHeight; j += 40) {
            if ((i / 40 + j / 40) % 2 == 0) {
                DrawRectangle(i, j, 40, 40, Color{20, 20, 35, 255});
            }
        }
    }
}

void Game::drawBoard() {
    int boardW = width_ * cellSize_;
    int boardH = height_ * cellSize_;
    int bx = GameConfig::kBoardOffsetX - 3;
    int by = GameConfig::kBoardOffsetY - 3;

    DrawRectangle(bx, by, boardW + 6, boardH + 6, Color{0, 200, 150, 255});
    DrawRectangle(bx + 2, by + 2, boardW + 2, boardH + 2, Color{15, 15, 25, 255});
}

void Game::drawGrid() {
    for (int x = 0; x <= width_; ++x) {
        int px = GameConfig::kBoardOffsetX + x * cellSize_;
        DrawLine(px, GameConfig::kBoardOffsetY, px, GameConfig::kBoardOffsetY + height_ * cellSize_, Color{40, 40, 60, 80});
    }
    for (int y = 0; y <= height_; ++y) {
        int py = GameConfig::kBoardOffsetY + y * cellSize_;
        DrawLine(GameConfig::kBoardOffsetX, py, GameConfig::kBoardOffsetX + width_ * cellSize_, py, Color{40, 40, 60, 80});
    }
}

static Rectangle makeRect(float x, float y, float w, float h) {
    Rectangle r;
    r.x = x; r.y = y; r.width = w; r.height = h;
    return r;
}

static Color makeColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    Color c;
    c.r = r; c.g = g; c.b = b; c.a = a;
    return c;
}

void Game::drawSnake() {
    const auto& body = snake_.getBody();
    for (int i = static_cast<int>(body.size()) - 1; i >= 0; --i) {
        float px = static_cast<float>(GameConfig::kBoardOffsetX + body[i].x * cellSize_);
        float py = static_cast<float>(GameConfig::kBoardOffsetY + body[i].y * cellSize_);
        float cs = static_cast<float>(cellSize_);

        if (i == 0) {
            Color headColor = makeColor(0, 255, 150, 255);
            DrawRectangleRounded(makeRect(px + 1, py + 1, cs - 2, cs - 2), 0.3f, 8, headColor);
            DrawRectangleRounded(makeRect(px + 3, py + 3, cs - 6, cs - 6), 0.3f, 8, makeColor(100, 255, 200, 255));

            float eyeSize = cs / 6.0f;
            float eyeOffset = cs / 4.0f;
            float cx = px + cs / 2.0f;
            float cy = py + cs / 2.0f;
            float ex1 = cx, ey1 = cy, ex2 = cx, ey2 = cy;

            switch (snake_.getDirection()) {
                case Direction::Right: ex1 = cx + eyeOffset; ey1 = cy - eyeOffset; ex2 = cx + eyeOffset; ey2 = cy + eyeOffset; break;
                case Direction::Left:  ex1 = cx - eyeOffset; ey1 = cy - eyeOffset; ex2 = cx - eyeOffset; ey2 = cy + eyeOffset; break;
                case Direction::Up:    ex1 = cx - eyeOffset; ey1 = cy - eyeOffset; ex2 = cx + eyeOffset; ey2 = cy - eyeOffset; break;
                case Direction::Down:  ex1 = cx - eyeOffset; ey1 = cy + eyeOffset; ex2 = cx + eyeOffset; ey2 = cy + eyeOffset; break;
                default: break;
            }
            DrawCircle(ex1, ey1, eyeSize, WHITE);
            DrawCircle(ex2, ey2, eyeSize, WHITE);
            DrawCircle(ex1, ey1, eyeSize * 0.5f, BLACK);
            DrawCircle(ex2, ey2, eyeSize * 0.5f, BLACK);
        } else {
            float t = 1.0f - static_cast<float>(i) / static_cast<float>(body.size());
            unsigned char g = static_cast<unsigned char>(180 + 75 * t);
            unsigned char b = static_cast<unsigned char>(80 + 70 * t);
            Color bodyColor = makeColor(0, g, b, 255);
            DrawRectangleRounded(makeRect(px + 2, py + 2, cs - 4, cs - 4), 0.25f, 6, bodyColor);
        }
    }
}

void Game::drawFood() {
    float px = static_cast<float>(GameConfig::kBoardOffsetX + food_.getPosition().x * cellSize_);
    float py = static_cast<float>(GameConfig::kBoardOffsetY + food_.getPosition().y * cellSize_);
    float cs = static_cast<float>(cellSize_);
    float cx = px + cs / 2.0f;
    float cy = py + cs / 2.0f;
    float pulse = 1.0f + 0.15f * sinf(frameCounter_ * 0.15f);
    float radius = (cs / 2.0f - 3.0f) * pulse;

    DrawCircle(cx, cy, radius + 3.0f, makeColor(255, 200, 0, 80));
    DrawCircle(cx, cy, radius, makeColor(255, 220, 50, 255));
    DrawCircle(cx, cy, radius * 0.5f, makeColor(255, 255, 180, 255));
}

void Game::drawParticles() {
    for (const auto& p : particles_) {
        float alpha = p.life / p.maxLife;
        DrawCircle(p.x, p.y, 3.0f * alpha, makeColor(p.color.r, p.color.g, p.color.b, static_cast<unsigned char>(alpha * 255.0f)));
    }
}

void Game::spawnParticles(int gridX, int gridY) {
    float cx = static_cast<float>(GameConfig::kBoardOffsetX + gridX * cellSize_ + cellSize_ / 2);
    float cy = static_cast<float>(GameConfig::kBoardOffsetY + gridY * cellSize_ + cellSize_ / 2);

    for (int i = 0; i < 15; ++i) {
        Particle p;
        p.x = cx;
        p.y = cy;
        float angle = static_cast<float>(i) / 15.0f * 3.14159f * 2.0f;
        float speed = 80.0f + static_cast<float>(rand() % 60);
        p.vx = cosf(angle) * speed;
        p.vy = sinf(angle) * speed;
        p.life = 0.5f + static_cast<float>(rand() % 50) / 100.0f;
        p.maxLife = p.life;
        unsigned char rg = static_cast<unsigned char>(200 + rand() % 55);
        p.color = makeColor(255, rg, 50, 255);
        particles_.push_back(p);
    }
}

void Game::drawHUD() {
    int boardW = width_ * cellSize_;
    int hudY = GameConfig::kBoardOffsetY + height_ * cellSize_ + 20;

    std::string scoreText = "SCORE: " + std::to_string(score_);
    std::string highText = "HIGH: " + std::to_string(scoreManager_.getHighScore());
    std::string levelText = "LEVEL: " + std::to_string(getLevel());

    DrawText(scoreText.c_str(), GameConfig::kBoardOffsetX, hudY, 22, makeColor(0, 200, 150, 255));
    DrawText(highText.c_str(), GameConfig::kBoardOffsetX + boardW / 2 - MeasureText(highText.c_str(), 22) / 2, hudY, 22, makeColor(200, 200, 200, 255));
    DrawText(levelText.c_str(), GameConfig::kBoardOffsetX + boardW - MeasureText(levelText.c_str(), 22), hudY, 22, makeColor(255, 200, 50, 255));

    std::string controls = "P: Pause | Q: Quit | Arrows/WASD: Move";
    DrawText(controls.c_str(), GameConfig::kBoardOffsetX + boardW / 2 - MeasureText(controls.c_str(), 14) / 2,
             GameConfig::kScreenHeight - 25, 14, makeColor(100, 100, 120, 255));
}

void Game::drawMenu() {
    int cx = GameConfig::kScreenWidth / 2;
    int cy = GameConfig::kScreenHeight / 2;

    float glow = 0.5f + 0.5f * sinf(frameCounter_ * 0.05f);

    DrawText(GameConfig::kGameTitle, cx - MeasureText(GameConfig::kGameTitle, 60) / 2, cy - 100, 60, makeColor(0, 200, 150, 255));

    std::string startText = "PRESS ENTER OR SPACE TO START";
    unsigned char alpha = static_cast<unsigned char>(150 + 105 * glow);
    DrawText(startText.c_str(), cx - MeasureText(startText.c_str(), 24) / 2, cy, 24, makeColor(200, 200, 200, alpha));

    std::string controls[] = {
        "Arrow Keys / WASD - Move",
        "P - Pause",
        "Q / ESC - Quit"
    };

    for (int i = 0; i < 3; ++i) {
        DrawText(controls[i].c_str(), cx - MeasureText(controls[i].c_str(), 18) / 2, cy + 60 + i * 30, 18, makeColor(150, 150, 170, 255));
    }

    int highScore = scoreManager_.getHighScore();
    std::string hsText = "HIGH SCORE: " + std::to_string(highScore);
    DrawText(hsText.c_str(), cx - MeasureText(hsText.c_str(), 20) / 2, cy + 170, 20, makeColor(255, 200, 50, 255));
}

void Game::drawPaused() {
    DrawRectangle(0, 0, GameConfig::kScreenWidth, GameConfig::kScreenHeight, makeColor(0, 0, 0, 150));

    std::string text = "PAUSED";
    DrawText(text.c_str(), GameConfig::kScreenWidth / 2 - MeasureText(text.c_str(), 48) / 2,
             GameConfig::kScreenHeight / 2 - 24, 48, makeColor(0, 200, 150, 255));

    std::string sub = "Press P to resume";
    DrawText(sub.c_str(), GameConfig::kScreenWidth / 2 - MeasureText(sub.c_str(), 20) / 2,
             GameConfig::kScreenHeight / 2 + 30, 20, makeColor(180, 180, 180, 255));
}

void Game::drawGameOver() {
    DrawRectangle(0, 0, GameConfig::kScreenWidth, GameConfig::kScreenHeight, makeColor(0, 0, 0, 180));

    std::string text = "GAME OVER";
    DrawText(text.c_str(), GameConfig::kScreenWidth / 2 - MeasureText(text.c_str(), 48) / 2,
             GameConfig::kScreenHeight / 2 - 80, 48, makeColor(255, 80, 80, 255));

    std::string scoreText = "SCORE: " + std::to_string(score_);
    DrawText(scoreText.c_str(), GameConfig::kScreenWidth / 2 - MeasureText(scoreText.c_str(), 32) / 2,
             GameConfig::kScreenHeight / 2 - 20, 32, WHITE);

    std::string hsText = "HIGH SCORE: " + std::to_string(scoreManager_.getHighScore());
    DrawText(hsText.c_str(), GameConfig::kScreenWidth / 2 - MeasureText(hsText.c_str(), 24) / 2,
             GameConfig::kScreenHeight / 2 + 20, 24, makeColor(255, 200, 50, 255));

    std::string restart = "R - Restart";
    std::string quit = "Q - Quit";
    DrawText(restart.c_str(), GameConfig::kScreenWidth / 2 - MeasureText(restart.c_str(), 22) / 2,
             GameConfig::kScreenHeight / 2 + 70, 22, makeColor(0, 200, 150, 255));
    DrawText(quit.c_str(), GameConfig::kScreenWidth / 2 - MeasureText(quit.c_str(), 22) / 2,
             GameConfig::kScreenHeight / 2 + 100, 22, makeColor(200, 200, 200, 255));
}

void Game::resetGame() {
    snake_ = Snake(width_ / 2, height_ / 2);
    food_.spawn(width_, height_, snake_.getBody());
    score_ = 0;
    state_ = State::Playing;
    particles_.clear();
}

int Game::getSpeed() const {
    int speed = GameConfig::kInitialSpeed + (score_ / GameConfig::kScorePerFood);
    return std::min(speed, GameConfig::kMaxSpeed);
}

int Game::getLevel() const {
    return (score_ / GameConfig::kScorePerFood) / 5 + 1;
}

}
