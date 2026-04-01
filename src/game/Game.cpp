#include "game/Game.h"
#include <cmath>
#include <algorithm>
#include <sstream>
#include <filesystem>

namespace snakepro {

Game::Game()
    : window_(sf::VideoMode(Config::kScreenWidth, Config::kScreenHeight), Config::kTitle, sf::Style::Titlebar | sf::Style::Close),
      snake_(gridW_ / 2, gridH_ / 2) {
    window_.setFramerateLimit(60);

    std::filesystem::path fontPath = std::filesystem::current_path() / "assets" / "font.ttf";
    if (font_.openFromFile(fontPath.string())) {
        Renderer::setFont(font_);
    }

    int boardW = gridW_ * cellSize_;
    int boardH = gridH_ * cellSize_;
    boardX_ = (Config::kScreenWidth - boardW) / 2;
    boardY_ = 100;

    food_.spawn(gridW_, gridH_, snake_.getBody());
    highScore_ = scoreManager_.getHighScore();
}

void Game::run() {
    sf::Clock clock;

    while (window_.isOpen()) {
        sf::Event event;
        while (window_.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window_.close();
            }
        }

        handleInput();
        update();
        frameTime_ += clock.restart().asSeconds();
        render();
    }
}

void Game::handleInput() {
    if (state_ == State::Menu) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            state_ = State::Playing;
            sf::sleep(sf::milliseconds(200));
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::H)) {
            state_ = State::HighScores;
            sf::sleep(sf::milliseconds(200));
        }
        return;
    }

    if (state_ == State::HighScores) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) || sf::Keyboard::isKeyPressed(sf::Keyboard::H)) {
            state_ = State::Menu;
            sf::sleep(sf::milliseconds(200));
        }
        return;
    }

    if (state_ == State::GameOver) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
            resetGame();
            sf::sleep(sf::milliseconds(200));
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            state_ = State::Menu;
            sf::sleep(sf::milliseconds(200));
        }
        return;
    }

    if (state_ == State::Paused) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::P) || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            state_ = State::Playing;
            sf::sleep(sf::milliseconds(200));
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
            state_ = State::Menu;
            sf::sleep(sf::milliseconds(200));
        }
        return;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::P) || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        state_ = State::Paused;
        sf::sleep(sf::milliseconds(200));
        return;
    }

    static sf::Keyboard::Key lastDir = sf::Keyboard::Right;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        if (lastDir != sf::Keyboard::Down) { snake_.setDirection(Direction::Up); lastDir = sf::Keyboard::Up; }
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        if (lastDir != sf::Keyboard::Up) { snake_.setDirection(Direction::Down); lastDir = sf::Keyboard::Down; }
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        if (lastDir != sf::Keyboard::Right) { snake_.setDirection(Direction::Left); lastDir = sf::Keyboard::Left; }
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        if (lastDir != sf::Keyboard::Left) { snake_.setDirection(Direction::Right); lastDir = sf::Keyboard::Right; }
    }
}

void Game::update() {
    float dt = frameTime_;
    if (dt > 0.1f) dt = 0.1f;

    pulseTimer_ += dt;
    if (shakeAmount_ > 0) {
        shakeAmount_ *= 0.9f;
        if (shakeAmount_ < 0.5f) shakeAmount_ = 0;
    }

    particles_.update(dt);

    if (state_ == State::Playing) {
        moveTimer_ += dt;
        float interval = getInterval();

        if (moveTimer_ >= interval) {
            moveTimer_ = 0;
            snake_.move();

            if (snake_.getHead() == food_.getPosition()) {
                score_ += Config::kScorePerFood;
                if (score_ > highScore_) {
                    highScore_ = score_;
                    isNewHighScore_ = true;
                }

                sf::Vector2f foodPos(
                    static_cast<float>(boardX_ + food_.getPosition().x * cellSize_ + cellSize_ / 2),
                    static_cast<float>(boardY_ + food_.getPosition().y * cellSize_ + cellSize_ / 2)
                );
                spawnEatEffect(foodPos);
                shakeAmount_ = 6;

                snake_.grow();
                food_.spawn(gridW_, gridH_, snake_.getBody());
            }

            if (snake_.checkSelfCollision() || snake_.checkWallCollision(gridW_, gridH_)) {
                state_ = State::GameOver;
                scoreManager_.addScore(score_);
                spawnDeathEffect();
                shakeAmount_ = 15;
            }
        }
    }
}

void Game::render() {
    window_.clear(Colors::bg());

    sf::View view = window_.getView();
    if (shakeAmount_ > 0) {
        float sx = (static_cast<float>(rand() % 200) / 100.f - 1.f) * shakeAmount_;
        float sy = (static_cast<float>(rand() % 200) / 100.f - 1.f) * shakeAmount_;
        view.move(sx, sy);
        window_.setView(view);
    }

    drawBackground();
    drawBoard();

    if (state_ == State::Menu) {
        drawMenu();
    } else if (state_ == State::HighScores) {
        drawHighScores();
    } else {
        drawGrid();
        drawSnake();
        drawFood();
        drawHUD();
        particles_.render(window_);

        if (state_ == State::Paused) drawPaused();
        if (state_ == State::GameOver) drawGameOver();
    }

    window_.setView(window_.getDefaultView());
    window_.display();
}

void Game::drawBackground() {
    sf::RectangleShape bg(sf::Vector2f(Config::kScreenWidth, Config::kScreenHeight));
    bg.setFillColor(Colors::bg());
    window_.draw(bg);

    for (int y = 0; y < Config::kScreenHeight; y += 4) {
        float t = y / static_cast<float>(Config::kScreenHeight);
        sf::Color c;
        c.r = static_cast<sf::Uint8>(13 + t * 8);
        c.g = static_cast<sf::Uint8>(13 + t * 6);
        c.b = static_cast<sf::Uint8>(22 + t * 14);
        c.a = 255;
        sf::RectangleShape line(sf::Vector2f(Config::kScreenWidth, 4));
        line.setPosition(0, static_cast<float>(y));
        line.setFillColor(c);
        window_.draw(line);
    }

    float time = pulseTimer_ * 0.5f;
    for (int i = 0; i < 6; ++i) {
        float x = Config::kScreenWidth * 0.2f + std::sin(time + i * 1.5f) * Config::kScreenWidth * 0.3f;
        float y = Config::kScreenHeight * 0.5f + std::cos(time * 0.7f + i * 2.0f) * Config::kScreenHeight * 0.3f;
        sf::CircleShape orb(80);
        orb.setPosition(x - 80, y - 80);
        orb.setFillColor(sf::Color(0, 180, 140, 4));
        window_.draw(orb);
    }
}

void Game::drawBoard() {
    int bw = gridW_ * cellSize_;
    int bh = gridH_ * cellSize_;
    int pad = 4;

    sf::RectangleShape outer(sf::Vector2f(bw + pad * 2 + 6, bh + pad * 2 + 6));
    outer.setPosition(boardX_ - pad - 3, boardY_ - pad - 3);
    outer.setFillColor(Colors::border());
    window_.draw(outer);

    sf::RectangleShape mid(sf::Vector2f(bw + pad * 2, bh + pad * 2));
    mid.setPosition(boardX_ - pad, boardY_ - pad);
    mid.setFillColor(Colors::bgLight());
    window_.draw(mid);

    sf::RectangleShape inner(sf::Vector2f(bw, bh));
    inner.setPosition(boardX_, boardY_);
    inner.setFillColor(Colors::board());
    window_.draw(inner);
}

void Game::drawGrid() {
    for (int x = 0; x <= gridW_; ++x) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(boardX_ + x * cellSize_, boardY_), Colors::grid()),
            sf::Vertex(sf::Vector2f(boardX_ + x * cellSize_, boardY_ + gridH_ * cellSize_), Colors::grid())
        };
        window_.draw(line, 2, sf::Lines);
    }
    for (int y = 0; y <= gridH_; ++y) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(boardX_, boardY_ + y * cellSize_), Colors::grid()),
            sf::Vertex(sf::Vector2f(boardX_ + gridW_ * cellSize_, boardY_ + y * cellSize_), Colors::grid())
        };
        window_.draw(line, 2, sf::Lines);
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
            float pulse = 1.0f + 0.04f * std::sin(pulseTimer_ * 5);
            float size = (cs - pad * 2) * pulse;
            float offset = (cs - size) / 2;

            sf::RectangleShape head(sf::Vector2f(size, size));
            head.setPosition(px + offset, py + offset);
            head.setFillColor(Colors::snakeHead());
            head.setRadius(cs * 0.15f);
            window_.draw(head);

            sf::RectangleShape inner(sf::Vector2f(size - 4, size - 4));
            inner.setPosition(px + offset + 2, py + offset + 2);
            inner.setFillColor(sf::Color(100, 255, 220, 180));
            inner.setRadius(cs * 0.12f);
            window_.draw(inner);

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

            sf::CircleShape eye1(eyeSize);
            eye1.setOrigin(eyeSize, eyeSize);
            eye1.setPosition(ex1, ey1);
            eye1.setFillColor(sf::Color::White);
            window_.draw(eye1);

            sf::CircleShape eye2(eyeSize);
            eye2.setOrigin(eyeSize, eyeSize);
            eye2.setPosition(ex2, ey2);
            eye2.setFillColor(sf::Color::White);
            window_.draw(eye2);

            sf::CircleShape pupil1(eyeSize * 0.5f);
            pupil1.setOrigin(eyeSize * 0.5f, eyeSize * 0.5f);
            pupil1.setPosition(ex1, ey1);
            pupil1.setFillColor(sf::Color::Black);
            window_.draw(pupil1);

            sf::CircleShape pupil2(eyeSize * 0.5f);
            pupil2.setOrigin(eyeSize * 0.5f, eyeSize * 0.5f);
            pupil2.setPosition(ex2, ey2);
            pupil2.setFillColor(sf::Color::Black);
            window_.draw(pupil2);
        } else {
            float t = 1.0f - static_cast<float>(i) / static_cast<float>(body.size());
            sf::Color col = Colors::snakeBody(t);
            float shrink = pad + (1.0f - t) * 3;
            sf::RectangleShape seg(sf::Vector2f(cs - shrink * 2, cs - shrink * 2));
            seg.setPosition(px + shrink, py + shrink);
            seg.setFillColor(col);
            seg.setRadius(cs * 0.12f);
            window_.draw(seg);
        }
    }
}

void Game::drawFood() {
    float px = static_cast<float>(boardX_ + food_.getPosition().x * cellSize_);
    float py = static_cast<float>(boardY_ + food_.getPosition().y * cellSize_);
    float cs = static_cast<float>(cellSize_);
    float cx = px + cs / 2.0f;
    float cy = py + cs / 2.0f;
    float pulse = 1.0f + 0.12f * std::sin(pulseTimer_ * 6);
    float radius = (cs / 2.0f - 4.0f) * pulse;

    float glowR = cs * 1.5f + 10 * std::sin(pulseTimer_ * 3);
    sf::Color glowCol = Colors::foodGlow();
    glowCol.a = static_cast<sf::Uint8>(40 + 30 * std::sin(pulseTimer_ * 3));
    sf::CircleShape glow(glowR);
    glow.setOrigin(glowR, glowR);
    glow.setPosition(cx, cy);
    glow.setFillColor(glowCol);
    window_.draw(glow);

    sf::CircleShape food(radius);
    food.setOrigin(radius, radius);
    food.setPosition(cx, cy);
    food.setFillColor(Colors::food());
    window_.draw(food);

    sf::CircleShape highlight(radius * 0.4f);
    highlight.setOrigin(radius * 0.4f, radius * 0.4f);
    highlight.setPosition(cx, cy);
    highlight.setFillColor(sf::Color(255, 255, 230, 255));
    window_.draw(highlight);
}

void Game::drawHUD() {
    int bw = gridW_ * cellSize_;
    int hudY = boardY_ + gridH_ * cellSize_ + 20;

    std::string scoreText = "SCORE: " + std::to_string(score_);
    std::string highText = "HIGH: " + std::to_string(highScore_);
    std::string levelText = "LEVEL: " + std::to_string(getLevel());

    Renderer::drawTextShadow(window_, scoreText, static_cast<float>(boardX_), static_cast<float>(hudY), 20, Colors::accent(), Colors::bg());
    Renderer::drawTextShadow(window_, highText, static_cast<float>(boardX_ + bw / 2 - 60), static_cast<float>(hudY), 20, Colors::gold(), Colors::bg());
    Renderer::drawTextShadow(window_, levelText, static_cast<float>(boardX_ + bw - 90), static_cast<float>(hudY), 20, Colors::textDim(), Colors::bg());

    std::string controls = "P: Pause  |  Q: Menu  |  Arrows/WASD: Move";
    Renderer::drawTextCentered(window_, controls, Config::kScreenWidth / 2.f, Config::kScreenHeight - 20, 13, Colors::textMuted());
}

void Game::drawMenu() {
    float cx = Config::kScreenWidth / 2.f;
    float cy = Config::kScreenHeight / 2.f;
    float glow = 0.5f + 0.5f * std::sin(pulseTimer_ * 2);

    Renderer::drawTextShadow(window_, Config::kTitle, cx - 200, cy - 140, 72, Colors::accentBright(), Colors::bg(), 3);

    std::string sub = "A Modern Snake Experience";
    Renderer::drawTextCentered(window_, sub, cx, cy - 70, 18, Colors::textDim());

    std::string startText = "PRESS ENTER OR SPACE TO START";
    sf::Color startCol(220, 220, 230, static_cast<sf::Uint8>(140 + 115 * glow));
    Renderer::drawTextCentered(window_, startText, cx, cy + 10, 22, startCol);

    std::string hsText = "HIGH SCORE: " + std::to_string(highScore_);
    Renderer::drawTextCentered(window_, hsText, cx, cy + 60, 18, Colors::gold());

    std::string hint = "Press H for High Scores";
    Renderer::drawTextCentered(window_, hint, cx, cy + 100, 14, Colors::textMuted());

    std::string controls[] = {
        "Arrow Keys / WASD  -  Move",
        "P / ESC            -  Pause",
        "Q                  -  Quit to Menu"
    };

    for (int i = 0; i < 3; ++i) {
        Renderer::drawTextCentered(window_, controls[i], cx, cy + 155 + i * 28, 16, Colors::textMuted());
    }

    Renderer::drawTextCentered(window_, Config::kVersion, cx, Config::kScreenHeight - 25, 12, Colors::textMuted());
}

void Game::drawPaused() {
    sf::RectangleShape overlay(sf::Vector2f(Config::kScreenWidth, Config::kScreenHeight));
    overlay.setFillColor(Colors::overlay());
    window_.draw(overlay);

    float cx = Config::kScreenWidth / 2.f;
    float cy = Config::kScreenHeight / 2.f;

    Renderer::drawTextShadow(window_, "PAUSED", cx - 120, cy - 50, 56, Colors::accent(), Colors::bg(), 3);
    Renderer::drawTextCentered(window_, "Press P to Resume", cx, cy + 30, 20, Colors::textDim());
}

void Game::drawGameOver() {
    sf::RectangleShape overlay(sf::Vector2f(Config::kScreenWidth, Config::kScreenHeight));
    overlay.setFillColor(Colors::overlay());
    window_.draw(overlay);

    float cx = Config::kScreenWidth / 2.f;
    float cy = Config::kScreenHeight / 2.f;

    Renderer::drawTextShadow(window_, "GAME OVER", cx - 160, cy - 110, 56, Colors::danger(), Colors::bg(), 3);

    std::string scoreText = "SCORE: " + std::to_string(score_);
    Renderer::drawTextCentered(window_, scoreText, cx, cy - 30, 32, Colors::text());

    std::string hsText = "BEST: " + std::to_string(highScore_);
    Renderer::drawTextCentered(window_, hsText, cx, cy + 15, 22, Colors::gold());

    if (isNewHighScore_) {
        std::string newHs = "NEW HIGH SCORE!";
        float pulse = 0.7f + 0.3f * std::sin(pulseTimer_ * 4);
        sf::Color col(255, 200, 50, static_cast<sf::Uint8>(255 * pulse));
        Renderer::drawTextCentered(window_, newHs, cx, cy + 55, 18, col);
    }

    Renderer::drawTextCentered(window_, "R - Play Again", cx, cy + 105, 20, Colors::accent());
    Renderer::drawTextCentered(window_, "Q - Main Menu", cx, cy + 140, 18, Colors::textDim());
}

void Game::drawHighScores() {
    float cx = Config::kScreenWidth / 2.f;
    float cy = Config::kScreenHeight / 2.f;

    Renderer::drawTextShadow(window_, "HIGH SCORES", cx - 160, cy - 190, 48, Colors::gold(), Colors::bg(), 3);

    const auto& scores = scoreManager_.getScores();
    int startY = cy - 130;

    for (int i = 0; i < 10; ++i) {
        int y = startY + i * 32;
        sf::Color rowCol = (i % 2 == 0) ? sf::Color(30, 30, 50, 100) : sf::Color(20, 20, 35, 100);
        sf::RectangleShape row(sf::Vector2f(420, 28));
        row.setPosition(cx - 210, y - 14);
        row.setFillColor(rowCol);
        window_.draw(row);

        std::string rank = "#" + std::to_string(i + 1);
        std::string entryText;
        if (i < static_cast<int>(scores.size())) {
            entryText = std::to_string(scores[i].score) + " pts  -  " + scores[i].date;
        } else {
            entryText = "---";
        }

        sf::Color rankCol = (i < 3) ? Colors::gold() : Colors::textDim();
        Renderer::drawTextShadow(window_, rank, cx - 190, static_cast<float>(y), 16, rankCol, Colors::bg());
        Renderer::drawTextShadow(window_, entryText, cx - 110, static_cast<float>(y), 16, Colors::text(), Colors::bg());
    }

    Renderer::drawTextCentered(window_, "Press ESC or ENTER to go back", cx, cy + 210, 16, Colors::textMuted());
}

void Game::resetGame() {
    snake_ = Snake(gridW_ / 2, gridH_ / 2);
    food_.spawn(gridW_, gridH_, snake_.getBody());
    score_ = 0;
    isNewHighScore_ = false;
    state_ = State::Playing;
    moveTimer_ = 0;
    particles_.clear();
}

float Game::getInterval() const {
    float interval = Config::kInitialInterval - (score_ / Config::kScorePerFood) * 0.008f;
    return std::max(interval, Config::kMinInterval);
}

int Game::getLevel() const {
    return (score_ / Config::kScorePerFood) / 5 + 1;
}

void Game::spawnEatEffect(sf::Vector2f pos) {
    particles_.emitBurst(pos, 20, Colors::food(), 120, 0.7f, 5);
    particles_.emit(pos, 8, Colors::accentBright(), 80, 0.5f, 3);
}

void Game::spawnDeathEffect() {
    const auto& body = snake_.getBody();
    for (int i = 0; i < static_cast<int>(body.size()); ++i) {
        sf::Vector2f p(
            static_cast<float>(boardX_ + body[i].x * cellSize_ + cellSize_ / 2),
            static_cast<float>(boardY_ + body[i].y * cellSize_ + cellSize_ / 2)
        );
        particles_.emitBurst(p, 5, Colors::danger(), 60, 0.8f, 4);
    }
}

}
