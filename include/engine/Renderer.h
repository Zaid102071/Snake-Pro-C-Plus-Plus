#pragma once

#include <SFML/Graphics.hpp>
#include <string>

namespace snakepro {

class Renderer {
public:
    static void drawRoundedRect(sf::RenderTarget& target, float x, float y, float w, float h, float radius, sf::Color color);
    static void drawRoundedOutline(sf::RenderTarget& target, float x, float y, float w, float h, float radius, float thickness, sf::Color color);
    static void drawTextCentered(sf::RenderTarget& target, const std::string& text, float cx, float cy, unsigned int size, sf::Color color);
    static void drawTextShadow(sf::RenderTarget& target, const std::string& text, float x, float y, unsigned int size, sf::Color color, sf::Color shadow, float offset = 2);
    static void setFont(const sf::Font& font);
    static const sf::Font& getFont();

private:
    static const sf::Font* font_;
};

}
