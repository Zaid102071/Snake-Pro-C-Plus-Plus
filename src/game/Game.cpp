#include "game/Game.h"
#include <cmath>
#include <algorithm>
#include <sstream>

namespace snakepro {

Game::Game()
    : snake_(gridW_ / 2, gridH_ / 2),
      frameCounter_(0) {
    cellSize_ = 28;
    int boardPixelW = gridW_ * cellSize_;
    int boardPixelH = gridH_ * cellSize_;
    boardX_ = (Settings::kScreenWidth - boardPixelW) / 2;
    boardY_ = 90;
    food_.spawn(gridW_, gridH_, snake_.getBody());
    highScore_ = scoreManager_.getHighScore();
}

Game::~Game() {
    CloseWindow();
}

void Game::run() {
    InitWindow(Settings::kScreenWidth, Settings::kScreenHeight, Settings::kGameTitle);
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        handleInput();
        update();
        frameCounter_++;
        render();
    }
}

void Game::handleInput() {
    if (state_ == GameState::Menu) {
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
            state_ = GameState::Playing;
        } else if (IsKeyPressed(KEY_H)) {
            state_ = GameState::HighScores;
        }
        return;
    }

    if (state_ == GameState::HighScores) {
        if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_H)) {
            state_ = GameState::Menu;
        }
        return;
    }

    if (state_ == GameState::GameOver) {
        if (IsKeyPressed(KEY_R)) {
            resetGame();
        } else if (IsKeyPressed(KEY_Q) || IsKeyPressed(KEY_ESCAPE)) {
            state_ = GameState::Menu;
        }
        return;
    }

    if (state_ == GameState::Paused) {
        if (IsKeyPressed(KEY_P) || IsKeyPressed(KEY_ESCAPE)) {
            state_ = GameState::Playing;
        } else if (IsKeyPressed(KEY_Q)) {
            state_ = GameState::Menu;
        }
        return;
    }

    if (IsKeyPressed(KEY_P) || IsKeyPressed(KEY_ESCAPE)) {
        state_ = GameState::Paused;
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
    float dt = GetFrameTime();
    Renderer::updateScreenShake(dt);
    particles_.update(dt);

    if (state_ == GameState::Playing) {
        moveTimer_ += dt;
        float moveInterval = 1.0f / static_cast<float>(getSpeed());

        if (moveTimer_ >= moveInterval) {
            moveTimer_ = 0;
            snake_.move();

            if (snake_.getHead() == food_.getPosition()) {
                score_ += Settings::kScorePerFood;
                if (score_ > highScore_) {
                    highScore_ = score_;
                    isNewHighScore_ = true;
                }

                Vec2 foodPos = Vec2{
                    static_cast<float>(boardX_ + food_.getPosition().x * cellSize_ + cellSize_ / 2),
                    static_cast<float>(boardY_ + food_.getPosition().y * cellSize_ + cellSize_ / 2)
                };
                spawnEatEffect(foodPos);
                Renderer::setScreenShake(4);

                snake_.grow();
                food_.spawn(gridW_, gridH_, snake_.getBody());
            }

            if (snake_.checkSelfCollision() || snake_.checkWallCollision(gridW_, gridH_)) {
                state_ = GameState::GameOver;
                scoreManager_.addScore(score_);
                spawnDeathEffect();
                Renderer::setScreenShake(12);
            }
        }
    }
}

void Game::render() {
    BeginDrawing();
    ClearBackground(ColorPalette::bgDark());

    drawBackground();
    drawBoard();

    if (state_ == GameState::Menu) {
        drawMenu();
    } else if (state_ == GameState::HighScores) {
        drawHighScores();
    } else {
        drawGrid();
        drawSnake();
        drawFood();
        drawHUD();
        particles_.render();

        if (state_ == GameState::Paused) drawPaused();
        if (state_ == GameState::GameOver) drawGameOver();
    }

    EndDrawing();
}

void Game::drawBackground() {
    for (int y = 0; y < Settings::kScreenHeight; y += 4) {
        float t = y / static_cast<float>(Settings::kScreenHeight);
        Color c;
        c.r = static_cast<unsigned char>(12 + t * 8);
        c.g = static_cast<unsigned char>(12 + t * 6);
        c.b = static_cast<unsigned char>(20 + t * 12);
        c.a = 255;
        DrawRectangle(0, y, Settings::kScreenWidth, 4, c);
    }

    float time = frameCounter_ * 0.005f;
    for (int i = 0; i < 5; ++i) {
        float x = Settings::kScreenWidth * 0.2f + sinf(time + i * 1.5f) * Settings::kScreenWidth * 0.3f;
        float y = Settings::kScreenHeight * 0.5f + cosf(time * 0.7f + i * 2.0f) * Settings::kScreenHeight * 0.3f;
        DrawCircle(x, y, 80, Color{0, 180, 130, 3});
    }
}

void Game::drawBoard() {
    int bw = gridW_ * cellSize_;
    int bh = gridH_ * cellSize_;
    int pad = 4;

    ColorPalette::drawGlowCircle(0, 0, 0, ColorPalette::bgDark(), ColorPalette::bgDark(), 0);

    DrawRectangle(boardX_ - pad - 2, boardY_ - pad - 2, bw + pad * 2 + 4, bh + pad * 2 + 4, ColorPalette::border());
    DrawRectangle(boardX_ - pad, boardY_ - pad, bw + pad * 2, bh + pad * 2, ColorPalette::bgMid());
    DrawRectangle(boardX_, boardY_, bw, bh, ColorPalette::bgDark());
}

void Game::drawGrid() {
    for (int x = 0; x <= gridW_; ++x) {
        int px = boardX_ + x * cellSize_;
        DrawLine(px, boardY_, px, boardY_ + gridH_ * cellSize_, ColorPalette::gridLine());
    }
    for (int y = 0; y <= gridH_; ++y) {
        int py = boardY_ + y * cellSize_;
        DrawLine(boardX_, py, boardX_ + gridW_ * cellSize_, py, ColorPalette::gridLine());
    }
}

void Game::drawSnake() {
    const auto& body = snake_.getBody();
    for (int i = static_cast<int>(body.size()) - 1; i >= 0; --i) {
        float px = static_cast<float>(boardX_ + body[i].x * cellSize_);
        float py = static_cast<float>(boardY_ + body[i].y * cellSize_);
        float cs = static_cast<float>(cellSize_);
        float pad = 1.5f;

        if (i == 0) {
            float pulse = 1.0f + 0.05f * sinf(frameCounter_ * 0.2f);
            float size = (cs - pad * 2) * pulse;
            float offset = (cs - size) / 2;

            DrawRectangleRounded(
                Rectangle{px + offset, py + offset, size, size},
                0.35f, 8, ColorPalette::snakeHead());

            DrawRectangleRounded(
                Rectangle{px + offset + 2, py + offset + 2, size - 4, size - 4},
                0.3f, 8, Color{100, 255, 210, 200});

            float eyeSize = cs / 6.5f;
            float eyeOff = cs / 4.5f;
            float cx = px + cs / 2.0f;
            float cy = py + cs / 2.0f;
            float ex1 = cx, ey1 = cy, ex2 = cx, ey2 = cy;

            switch (snake_.getDirection()) {
                case Direction::Right: ex1 = cx + eyeOff; ey1 = cy - eyeOff; ex2 = cx + eyeOff; ey2 = cy + eyeOff; break;
                case Direction::Left:  ex1 = cx - eyeOff; ey1 = cy - eyeOff; ex2 = cx - eyeOff; ey2 = cy + eyeOff; break;
                case Direction::Up:    ex1 = cx - eyeOff; ey1 = cy - eyeOff; ex2 = cx + eyeOff; ey2 = cy - eyeOff; break;
                case Direction::Down:  ex1 = cx - eyeOff; ey1 = cy + eyeOff; ex2 = cx + eyeOff; ey2 = cy + eyeOff; break;
                default: break;
            }

            DrawCircle(ex1, ey1, eyeSize, WHITE);
            DrawCircle(ex2, ey2, eyeSize, WHITE);
            DrawCircle(ex1, ey1, eyeSize * 0.5f, BLACK);
            DrawCircle(ex2, ey2, eyeSize * 0.5f, BLACK);
        } else {
            float t = 1.0f - static_cast<float>(i) / static_cast<float>(body.size());
            Color col = ColorPalette::snakeBody(t);
            float shrink = pad + (1.0f - t) * 3;
            DrawRectangleRounded(
                Rectangle{px + shrink, py + shrink, cs - shrink * 2, cs - shrink * 2},
                0.3f, 6, col);
        }
    }
}

void Game::drawFood() {
    foodPulse_ += GetFrameTime() * 4;
    float px = static_cast<float>(boardX_ + food_.getPosition().x * cellSize_);
    float py = static_cast<float>(boardY_ + food_.getPosition().y * cellSize_);
    float cs = static_cast<float>(cellSize_);
    float cx = px + cs / 2.0f;
    float cy = py + cs / 2.0f;
    float pulse = 1.0f + 0.12f * sinf(foodPulse_);
    float radius = (cs / 2.0f - 4.0f) * pulse;

    float glowR = cs * 1.5f + 10 * sinf(foodPulse_ * 0.5f);
    Color glowCol = ColorPalette::foodGlow();
    glowCol.a = static_cast<unsigned char>(40 + 30 * sinf(foodPulse_ * 0.5f));
    DrawCircle(cx, cy, glowR, glowCol);
    DrawCircle(cx, cy, radius, ColorPalette::food());
    DrawCircle(cx, cy, radius * 0.45f, Color{255, 255, 220, 255});
}

void Game::drawHUD() {
    int bw = gridW_ * cellSize_;
    int hudY = boardY_ + gridH_ * cellSize_ + 18;

    std::string scoreText = "SCORE: " + std::to_string(score_);
    std::string highText = "HIGH: " + std::to_string(highScore_);
    std::string levelText = "LEVEL: " + std::to_string(getLevel());

    Renderer::drawTextShadow(scoreText.c_str(), static_cast<float>(boardX_), static_cast<float>(hudY), 20, ColorPalette::accent(), ColorPalette::bgDark());
    Renderer::drawTextShadow(highText.c_str(), static_cast<float>(boardX_ + bw / 2 - MeasureText(highText.c_str(), 20) / 2), static_cast<float>(hudY), 20, ColorPalette::gold(), ColorPalette::bgDark());
    Renderer::drawTextShadow(levelText.c_str(), static_cast<float>(boardX_ + bw - MeasureText(levelText.c_str(), 20)), static_cast<float>(hudY), 20, ColorPalette::gray(), ColorPalette::bgDark());

    std::string controls = "P: Pause  |  Q: Menu  |  Arrows/WASD: Move";
    Renderer::drawTextCentered(controls.c_str(), static_cast<float>(Settings::kScreenWidth / 2), static_cast<float>(Settings::kScreenHeight - 20), 13, ColorPalette::dimGray());
}

void Game::drawMenu() {
    float cx = static_cast<float>(Settings::kScreenWidth / 2);
    float cy = static_cast<float>(Settings::kScreenHeight / 2);
    menuPulse_ += GetFrameTime() * 3;

    float glow = 0.5f + 0.5f * sinf(menuPulse_ * 0.5f);

    Renderer::drawTextShadow(Settings::kGameTitle, cx - MeasureText(Settings::kGameTitle, 72) / 2, cy - 130, 72, ColorPalette::accentBright(), ColorPalette::bgDark(), 3);

    std::string sub = "A Modern Snake Experience";
    Renderer::drawTextCentered(sub.c_str(), cx, cy - 65, 18, ColorPalette::gray());

    std::string startText = "PRESS ENTER OR SPACE TO START";
    unsigned char alpha = static_cast<unsigned char>(140 + 115 * glow);
    Renderer::drawTextCentered(startText.c_str(), cx, cy + 10, 22, Color{220, 220, 230, alpha});

    std::string hsText = "HIGH SCORE: " + std::to_string(highScore_);
    Renderer::drawTextCentered(hsText.c_str(), cx, cy + 60, 18, ColorPalette::gold());

    std::string hint = "Press H for High Scores";
    Renderer::drawTextCentered(hint.c_str(), cx, cy + 100, 14, ColorPalette::dimGray());

    std::string controls[] = {
        "Arrow Keys / WASD  -  Move",
        "P / ESC            -  Pause",
        "Q                  -  Quit to Menu"
    };

    for (int i = 0; i < 3; ++i) {
        Renderer::drawTextCentered(controls[i].c_str(), cx, cy + 150 + i * 28, 16, ColorPalette::dimGray());
    }

    Renderer::drawTextCentered(Settings::kVersion, cx, static_cast<float>(Settings::kScreenHeight - 25), 12, ColorPalette::dimGray());
}

void Game::drawPaused() {
    DrawRectangle(0, 0, Settings::kScreenWidth, Settings::kScreenHeight, ColorPalette::overlay());

    float cx = static_cast<float>(Settings::kScreenWidth / 2);
    float cy = static_cast<float>(Settings::kScreenHeight / 2);

    std::string text = "PAUSED";
    Renderer::drawTextShadow(text.c_str(), cx - MeasureText(text.c_str(), 56) / 2, cy - 40, 56, ColorPalette::accent(), ColorPalette::bgDark(), 3);

    std::string sub = "Press P to Resume";
    Renderer::drawTextCentered(sub.c_str(), cx, cy + 30, 20, ColorPalette::gray());
}

void Game::drawGameOver() {
    DrawRectangle(0, 0, Settings::kScreenWidth, Settings::kScreenHeight, ColorPalette::overlay());

    float cx = static_cast<float>(Settings::kScreenWidth / 2);
    float cy = static_cast<float>(Settings::kScreenHeight / 2);

    std::string text = "GAME OVER";
    Renderer::drawTextShadow(text.c_str(), cx - MeasureText(text.c_str(), 56) / 2, cy - 100, 56, ColorPalette::danger(), ColorPalette::bgDark(), 3);

    std::string scoreText = "SCORE: " + std::to_string(score_);
    Renderer::drawTextCentered(scoreText.c_str(), cx, cy - 30, 32, ColorPalette::white());

    std::string hsText = "BEST: " + std::to_string(highScore_);
    Renderer::drawTextCentered(hsText.c_str(), cx, cy + 10, 22, ColorPalette::gold());

    if (isNewHighScore_) {
        std::string newHs = "NEW HIGH SCORE!";
        float pulse = 0.7f + 0.3f * sinf(frameCounter_ * 0.15f);
        Renderer::drawTextCentered(newHs.c_str(), cx, cy + 50, 18, Color{255, 200, 50, static_cast<unsigned char>(255 * pulse)});
    }

    std::string restart = "R - Play Again";
    std::string menu = "Q - Main Menu";
    Renderer::drawTextCentered(restart.c_str(), cx, cy + 100, 20, ColorPalette::accent());
    Renderer::drawTextCentered(menu.c_str(), cx, cy + 135, 18, ColorPalette::gray());
}

void Game::drawHighScores() {
    float cx = static_cast<float>(Settings::kScreenWidth / 2);
    float cy = static_cast<float>(Settings::kScreenHeight / 2);

    std::string title = "HIGH SCORES";
    Renderer::drawTextShadow(title.c_str(), cx - MeasureText(title.c_str(), 48) / 2, cy - 180, 48, ColorPalette::gold(), ColorPalette::bgDark(), 3);

    const auto& scores = scoreManager_.getScores();
    int startY = cy - 120;

    for (int i = 0; i < 10; ++i) {
        int y = startY + i * 30;
        Color rowCol = (i % 2 == 0) ? Color{30, 30, 50, 100} : Color{20, 20, 35, 100};
        DrawRectangle(cx - 200, y - 12, 400, 26, rowCol);

        std::string rank = "#" + std::to_string(i + 1);
        std::string entryText;
        if (i < static_cast<int>(scores.size())) {
            entryText = std::to_string(scores[i].score) + " pts  -  " + scores[i].date;
        } else {
            entryText = "---";
        }

        Color rankCol = (i < 3) ? ColorPalette::gold() : ColorPalette::gray();
        Renderer::drawTextShadow(rank.c_str(), cx - 180, static_cast<float>(y), 16, rankCol, ColorPalette::bgDark());
        Renderer::drawTextShadow(entryText.c_str(), cx - 100, static_cast<float>(y), 16, ColorPalette::white(), ColorPalette::bgDark());
    }

    std::string back = "Press ESC or ENTER to go back";
    Renderer::drawTextCentered(back.c_str(), cx, cy + 200, 16, ColorPalette::dimGray());
}

void Game::resetGame() {
    snake_ = Snake(gridW_ / 2, gridH_ / 2);
    food_.spawn(gridW_, gridH_, snake_.getBody());
    score_ = 0;
    isNewHighScore_ = false;
    state_ = GameState::Playing;
    moveTimer_ = 0;
    particles_.clear();
}

int Game::getSpeed() const {
    int speed = Settings::kInitialSpeed + (score_ / Settings::kScorePerFood);
    return std::min(speed, Settings::kMaxSpeed);
}

int Game::getLevel() const {
    return (score_ / Settings::kScorePerFood) / 5 + 1;
}

void Game::spawnEatEffect(Vec2 pos) {
    particles_.emitBurst(pos, 20, ColorPalette::food(), 120, 0.7f, 5);
    particles_.emit(pos, 8, ColorPalette::accentBright(), 80, 0.5f, 3);
}

void Game::spawnDeathEffect() {
    const auto& body = snake_.getBody();
    for (int i = 0; i < static_cast<int>(body.size()); ++i) {
        Vec2 p = Vec2{
            static_cast<float>(boardX_ + body[i].x * cellSize_ + cellSize_ / 2),
            static_cast<float>(boardY_ + body[i].y * cellSize_ + cellSize_ / 2)
        };
        particles_.emitBurst(p, 5, ColorPalette::danger(), 60, 0.8f, 4);
    }
}

}
