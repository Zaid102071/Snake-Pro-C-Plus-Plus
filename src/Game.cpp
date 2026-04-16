#include "Game.hpp"
#include <algorithm>

namespace snake {

Game::Game()
    : window_(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), WINDOW_TITLE,
              sf::Style::Titlebar | sf::Style::Close),
      renderer_(window_),
      snake_({GRID_COLS / 2, GRID_ROWS / 2}),
      state_(GameState::Menu),
      move_interval_(BASE_SPEED) {

    window_.setFramerateLimit(60);
    food_.spawn(snake_);
    move_clock_.start();
}

void Game::run() {
    while (window_.isOpen()) {
        handle_events();
        update();
        render();
    }
}

void Game::handle_events() {
    sf::Event event{};
    while (window_.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            score_.save_high_score();
            window_.close();
            return;
        }

        switch (state_) {
            case GameState::Menu:     handle_menu_input(event);     break;
            case GameState::Playing:  handle_play_input(event);     break;
            case GameState::Paused:   handle_pause_input(event);    break;
            case GameState::GameOver: handle_gameover_input(event); break;
        }
    }
}

void Game::handle_menu_input(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Enter) {
            reset_game();
            state_ = GameState::Playing;
        } else if (event.key.code == sf::Keyboard::Escape) {
            score_.save_high_score();
            window_.close();
        }
    }
}

void Game::handle_play_input(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
            case sf::Keyboard::Up:
            case sf::Keyboard::W:
                snake_.set_direction(Direction::Up); break;
            case sf::Keyboard::Down:
            case sf::Keyboard::S:
                snake_.set_direction(Direction::Down); break;
            case sf::Keyboard::Left:
            case sf::Keyboard::A:
                snake_.set_direction(Direction::Left); break;
            case sf::Keyboard::Right:
            case sf::Keyboard::D:
                snake_.set_direction(Direction::Right); break;
            case sf::Keyboard::P:
                state_ = GameState::Paused; break;
            case sf::Keyboard::Escape:
                state_ = GameState::Menu; break;
            default: break;
        }
    }
}

void Game::handle_pause_input(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::P) {
            state_ = GameState::Playing;
            move_clock_.restart();
        } else if (event.key.code == sf::Keyboard::Escape) {
            state_ = GameState::Menu;
        }
    }
}

void Game::handle_gameover_input(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Enter) {
            reset_game();
            state_ = GameState::Playing;
        } else if (event.key.code == sf::Keyboard::Escape) {
            state_ = GameState::Menu;
        }
    }
}

void Game::update() {
    if (state_ == GameState::Playing) {
        update_playing();
    }
}

void Game::update_playing() {
    if (move_clock_.getElapsedTime().asSeconds() < move_interval_) return;
    move_clock_.restart();

    snake_.update();

    if (snake_.head().x == 0 || snake_.head().x >= GRID_COLS - 1 ||
        snake_.head().y == 0 || snake_.head().y >= GRID_ROWS - 1 ||
        snake_.is_self_colliding()) {
        score_.save_high_score();
        state_ = GameState::GameOver;
        return;
    }

    if (snake_.head() == food_.position()) {
        snake_.grow();
        score_.add(10);
        food_.respawn(snake_);
        move_interval_ = std::max(0.04f, BASE_SPEED - score_.score() * SPEED_INCREMENT / 10.0f);
    }
}

void Game::render() {
    renderer_.clear();
    renderer_.draw_board();
    renderer_.draw_food(food_);
    renderer_.draw_snake(snake_);
    renderer_.draw_hud(score_);

    if (state_ != GameState::Playing) {
        renderer_.draw_state_overlay(state_, score_);
    }

    renderer_.display();
}

void Game::reset_game() {
    snake_.reset({GRID_COLS / 2, GRID_ROWS / 2});
    food_.spawn(snake_);
    score_.reset();
    move_interval_ = BASE_SPEED;
    move_clock_.restart();
}

}
