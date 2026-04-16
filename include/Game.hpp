#pragma once

#include <SFML/Graphics.hpp>
#include "Snake.hpp"
#include "Food.hpp"
#include "ScoreManager.hpp"
#include "Renderer.hpp"

namespace snake {

enum class GameState { Menu, Playing, Paused, GameOver };

class Game {
public:
    Game();

    void run();

private:
    void handle_events();
    void handle_menu_input(const sf::Event& event);
    void handle_play_input(const sf::Event& event);
    void handle_pause_input(const sf::Event& event);
    void handle_gameover_input(const sf::Event& event);

    void update();
    void update_playing();

    void render();

    void reset_game();

    sf::RenderWindow window_;
    Renderer         renderer_;
    Snake            snake_;
    Food             food_;
    ScoreManager     score_;
    GameState        state_;

    sf::Clock        move_clock_;
    float            move_interval_;
};

}
