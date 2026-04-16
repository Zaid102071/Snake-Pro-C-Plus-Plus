#include "Renderer.hpp"
#include "Game.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <sstream>
#include <iomanip>

namespace snake {

Renderer::Renderer(sf::RenderWindow& window)
    : window_(window) {
    font_.loadFromFile(FONT_PATH);
}

void Renderer::clear() {
    window_.clear(sf::Color(18, 18, 30));
}

void Renderer::draw_board() {
    sf::RectangleShape grid_line;
    grid_line.setFillColor(sf::Color(30, 30, 50));

    for (std::uint32_t x = 0; x <= WINDOW_WIDTH; x += CELL_SIZE) {
        grid_line.setSize({1.0f, static_cast<float>(WINDOW_HEIGHT)});
        grid_line.setPosition(static_cast<float>(x), 0.0f);
        window_.draw(grid_line);
    }

    for (std::uint32_t y = 0; y <= WINDOW_HEIGHT; y += CELL_SIZE) {
        grid_line.setSize({static_cast<float>(WINDOW_WIDTH), 1.0f});
        grid_line.setPosition(0.0f, static_cast<float>(y));
        window_.draw(grid_line);
    }

    sf::RectangleShape border;
    border.setFillColor(sf::Color(80, 80, 120));
    border.setSize({static_cast<float>(WINDOW_WIDTH), 2.0f});
    border.setPosition(0.0f, 0.0f);
    window_.draw(border);
}

void Renderer::draw_snake(const Snake& snake) {
    const auto& body = snake.body();
    const std::size_t total = body.size();

    for (std::size_t i = 0; i < total; ++i) {
        const auto& seg = body[i];
        sf::RectangleShape rect;
        rect.setSize({static_cast<float>(CELL_SIZE - 2), static_cast<float>(CELL_SIZE - 2)});
        rect.setPosition(static_cast<float>(seg.x * CELL_SIZE + 1),
                         static_cast<float>(seg.y * CELL_SIZE + 1));

        sf::Color color = snake_body_color(i, total);
        rect.setFillColor(color);

        if (i == 0) {
            rect.setOutlineThickness(1.0f);
            rect.setOutlineColor(sf::Color(255, 255, 255, 80));
        }

        window_.draw(rect);
    }

    if (!body.empty()) {
        const auto& head = body.front();
        float cx = static_cast<float>(head.x * CELL_SIZE + CELL_SIZE / 2);
        float cy = static_cast<float>(head.y * CELL_SIZE + CELL_SIZE / 2);

        float eye_offset = CELL_SIZE * 0.25f;
        float eye_radius = 3.0f;

        sf::CircleShape eye(eye_radius);
        eye.setFillColor(sf::Color::White);

        auto dir = snake.direction();
        float e1x = cx, e1y = cy, e2x = cx, e2y = cy;

        switch (dir) {
            case Direction::Right:
                e1x += eye_offset * 0.5f; e1y -= eye_offset;
                e2x += eye_offset * 0.5f; e2y += eye_offset;
                break;
            case Direction::Left:
                e1x -= eye_offset * 0.5f; e1y -= eye_offset;
                e2x -= eye_offset * 0.5f; e2y += eye_offset;
                break;
            case Direction::Up:
                e1x -= eye_offset; e1y -= eye_offset * 0.5f;
                e2x += eye_offset; e2y -= eye_offset * 0.5f;
                break;
            case Direction::Down:
                e1x -= eye_offset; e1y += eye_offset * 0.5f;
                e2x += eye_offset; e2y += eye_offset * 0.5f;
                break;
        }

        eye.setPosition(e1x - eye_radius, e1y - eye_radius);
        window_.draw(eye);
        eye.setPosition(e2x - eye_radius, e2y - eye_radius);
        window_.draw(eye);

        sf::CircleShape pupil(1.5f);
        pupil.setFillColor(sf::Color::Black);
        pupil.setPosition(e1x - 1.5f, e1y - 1.5f);
        window_.draw(pupil);
        pupil.setPosition(e2x - 1.5f, e2y - 1.5f);
        window_.draw(pupil);
    }
}

void Renderer::draw_food(const Food& food) {
    auto pos = food.position();
    float cx = static_cast<float>(pos.x * CELL_SIZE + CELL_SIZE / 2);
    float cy = static_cast<float>(pos.y * CELL_SIZE + CELL_SIZE / 2);

    float pulse = 1.0f + 0.15f * std::sin(pulse_clock_.getElapsedTime().asSeconds() * 5.0f);
    float radius = (CELL_SIZE / 2.0f - 2.0f) * pulse;

    sf::CircleShape glow(radius + 4.0f);
    glow.setFillColor(sf::Color(255, 50, 50, 40));
    glow.setPosition(cx - radius - 4.0f, cy - radius - 4.0f);
    window_.draw(glow);

    sf::CircleShape circle(radius);
    circle.setFillColor(sf::Color(255, 60, 60));
    circle.setPosition(cx - radius, cy - radius);
    window_.draw(circle);

    sf::CircleShape highlight(radius * 0.3f);
    highlight.setFillColor(sf::Color(255, 180, 180, 180));
    highlight.setPosition(cx - radius * 0.5f, cy - radius * 0.6f);
    window_.draw(highlight);
}

void Renderer::draw_hud(const ScoreManager& score) {
    draw_text("SCORE: " + std::to_string(score.score()), 20, 10.0f, 5.0f,
              sf::Color(200, 200, 255));

    draw_text("HI: " + std::to_string(score.high_score()), 20,
              static_cast<float>(WINDOW_WIDTH) - 10.0f, 5.0f,
              sf::Color(255, 215, 0), true);
}

void Renderer::draw_state_overlay(GameState state, const ScoreManager& score) {
    sf::RectangleShape overlay;
    overlay.setSize({static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT)});
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    window_.draw(overlay);

    switch (state) {
        case GameState::Menu:
            draw_text("SNAKE PRO", 72,
                      static_cast<float>(WINDOW_WIDTH) / 2, 160.0f,
                      sf::Color(0, 230, 118), true);

            draw_text("A modern take on the classic", 22,
                      static_cast<float>(WINDOW_WIDTH) / 2, 230.0f,
                      sf::Color(160, 160, 200), true);

            draw_text("Press ENTER to Start", 28,
                      static_cast<float>(WINDOW_WIDTH) / 2, 320.0f,
                      sf::Color(255, 255, 255), true);

            draw_text("WASD or Arrow Keys to Move", 18,
                      static_cast<float>(WINDOW_WIDTH) / 2, 380.0f,
                      sf::Color(140, 140, 170), true);

            draw_text("P to Pause  |  ESC to Quit", 18,
                      static_cast<float>(WINDOW_WIDTH) / 2, 410.0f,
                      sf::Color(140, 140, 170), true);
            break;

        case GameState::Paused:
            draw_text("PAUSED", 64,
                      static_cast<float>(WINDOW_WIDTH) / 2,
                      static_cast<float>(WINDOW_HEIGHT) / 2 - 30.0f,
                      sf::Color(255, 215, 0), true);

            draw_text("Press P to Resume", 24,
                      static_cast<float>(WINDOW_WIDTH) / 2,
                      static_cast<float>(WINDOW_HEIGHT) / 2 + 30.0f,
                      sf::Color(200, 200, 200), true);
            break;

        case GameState::GameOver:
            draw_text("GAME OVER", 64,
                      static_cast<float>(WINDOW_WIDTH) / 2,
                      static_cast<float>(WINDOW_HEIGHT) / 2 - 60.0f,
                      sf::Color(255, 60, 60), true);

            draw_text("Score: " + std::to_string(score.score()), 32,
                      static_cast<float>(WINDOW_WIDTH) / 2,
                      static_cast<float>(WINDOW_HEIGHT) / 2 + 10.0f,
                      sf::Color(255, 255, 255), true);

            if (score.score() >= score.high_score()) {
                draw_text("NEW HIGH SCORE!", 28,
                          static_cast<float>(WINDOW_WIDTH) / 2,
                          static_cast<float>(WINDOW_HEIGHT) / 2 + 50.0f,
                          sf::Color(255, 215, 0), true);
            }

            draw_text("Press ENTER to Restart", 22,
                      static_cast<float>(WINDOW_WIDTH) / 2,
                      static_cast<float>(WINDOW_HEIGHT) / 2 + 90.0f,
                      sf::Color(180, 180, 200), true);
            break;

        case GameState::Playing:
            break;
    }
}

void Renderer::display() {
    window_.display();
}

void Renderer::draw_text(const std::string& text, std::uint32_t size, float x, float y,
                         const sf::Color& color, bool center) {
    sf::Text txt;
    txt.setFont(font_);
    txt.setString(text);
    txt.setCharacterSize(size);
    txt.setFillColor(color);

    if (center) {
        sf::FloatRect bounds = txt.getLocalBounds();
        txt.setOrigin(bounds.left + bounds.width / 2.0f,
                      bounds.top + bounds.height / 2.0f);
    }

    txt.setPosition(x, y);
    window_.draw(txt);
}

sf::Color Renderer::snake_body_color(std::size_t index, std::size_t total) const {
    float t = static_cast<float>(index) / static_cast<float>(std::max(total - 1, std::size_t(1)));

    std::uint8_t r = static_cast<std::uint8_t>(0   + (0   - 0)   * t);
    std::uint8_t g = static_cast<std::uint8_t>(230 + (140 - 230) * t);
    std::uint8_t b = static_cast<std::uint8_t>(118 + (80  - 118) * t);

    return {r, g, b};
}

}
