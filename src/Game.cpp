#include "game/Game.h"
#include <thread>
#include <chrono>
#include <algorithm>

#ifdef _WIN32
#include <conio.h>
#else
#include <unistd.h>
#endif

namespace snakepro {

Game::Game()
    : snake_(Config::kDefaultWidth / 2, Config::kDefaultHeight / 2),
      food_(),
      renderer_(Config::kDefaultWidth, Config::kDefaultHeight),
      score_(0),
      running_(false),
      paused_(false),
      gameOver_(false),
      width_(Config::kDefaultWidth),
      height_(Config::kDefaultHeight) {
    food_.spawn(width_, height_, snake_.getBody());
}

Game::~Game() {
    InputHandler::restoreTerminal();
}

void Game::init() {
    renderer_.renderMenu();

    while (true) {
        Direction dir = input_.pollInput();
        if (dir != Direction::None) {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    running_ = true;
    paused_ = false;
    gameOver_ = false;
    score_ = 0;
}

void Game::run() {
    init();
    gameLoop();
}

int Game::getRawKey() {
#ifdef _WIN32
    if (_kbhit()) {
        return _getch();
    }
#else
    char c;
    if (read(STDIN_FILENO, &c, 1) > 0) {
        return static_cast<int>(c);
    }
#endif
    return 0;
}

void Game::gameLoop() {
    while (running_) {
        if (paused_) {
            int key = getRawKey();
            if (key == 'p' || key == 'P') {
                paused_ = false;
            } else if (key == 'q' || key == 'Q' || key == 27) {
                running_ = false;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        if (gameOver_) {
            int key = getRawKey();
            if (key == 'r' || key == 'R') {
                resetGame();
            } else if (key == 'q' || key == 'Q' || key == 27) {
                running_ = false;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        handleInput();
        update();
        render();
        std::this_thread::sleep_for(std::chrono::milliseconds(getSpeed()));
    }
}

void Game::handleInput() {
    Direction dir = input_.pollInput();

    if (dir == Direction::None) {
        int key = getRawKey();
        if (key == 'p' || key == 'P') {
            paused_ = true;
            return;
        } else if (key == 'q' || key == 'Q' || key == 27) {
            running_ = false;
            return;
        }
    }

    if (dir != Direction::None) {
        snake_.setDirection(dir);
    }
}

void Game::update() {
    snake_.move();

    if (snake_.getHead() == food_.getPosition()) {
        snake_.grow();
        score_ += Config::kScorePerFood;
        food_.spawn(width_, height_, snake_.getBody());
    }

    if (snake_.checkSelfCollision() || snake_.checkWallCollision(width_, height_)) {
        gameOver_ = true;
        scoreManager_.addScore(score_);
        renderer_.renderGameOver(score_, scoreManager_.getHighScore());
    }
}

void Game::render() {
    renderer_.render(snake_.getBody(), food_.getPosition(),
                     score_, scoreManager_.getHighScore(), getLevel(),
                     paused_, gameOver_);
}

void Game::resetGame() {
    snake_ = Snake(width_ / 2, height_ / 2);
    food_.spawn(width_, height_, snake_.getBody());
    score_ = 0;
    gameOver_ = false;
    paused_ = false;
}

int Game::getSpeed() const {
    int speed = Config::kInitialSpeed - (score_ / Config::kScorePerFood) * Config::kSpeedIncrement;
    return std::max(speed, Config::kMinSpeed);
}

int Game::getLevel() const {
    return (score_ / Config::kScorePerFood) / 5 + 1;
}

}
