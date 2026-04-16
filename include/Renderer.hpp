#pragma once

#include <SFML/Graphics.hpp>
#include "Snake.hpp"
#include "Food.hpp"
#include "ScoreManager.hpp"
#include <string>

namespace snake {

enum class GameState;

class Renderer {
public:
    explicit Renderer(sf::RenderWindow& window);

    void clear();
    void draw_board();
    void draw_snake(const Snake& snake);
    void draw_food(const Food& food);
    void draw_hud(const ScoreManager& score);
    void draw_state_overlay(GameState state, const ScoreManager& score);
    void display();

private:
    void draw_text(const std::string& text, std::uint32_t size, float x, float y,
                   const sf::Color& color, bool center = false);
    sf::Color snake_body_color(std::size_t index, std::size_t total) const;

    sf::RenderWindow& window_;
    sf::Font          font_;
    sf::Clock         pulse_clock_;
};

}
