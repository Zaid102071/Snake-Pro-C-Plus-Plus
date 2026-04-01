#include "engine/Renderer.h"
#include <cmath>

namespace snakepro {

const sf::Font* Renderer::font_ = nullptr;

void Renderer::setFont(const sf::Font& font) {
    font_ = &font;
}

const sf::Font& Renderer::getFont() {
    static sf::Font defaultFont;
    return font_ ? *font_ : defaultFont;
}

void Renderer::drawRoundedRect(sf::RenderTarget& target, float x, float y, float w, float h, float radius, sf::Color color) {
    sf::RectangleShape rect(sf::Vector2f(w, h));
    rect.setPosition(x, y);
    rect.setFillColor(color);
    rect.setRadius(radius);
    target.draw(rect);
}

void Renderer::drawRoundedOutline(sf::RenderTarget& target, float x, float y, float w, float h, float radius, float thickness, sf::Color color) {
    sf::RectangleShape rect(sf::Vector2f(w, h));
    rect.setPosition(x, y);
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(color);
    rect.setOutlineThickness(thickness);
    rect.setRadius(radius);
    target.draw(rect);
}

void Renderer::drawTextCentered(sf::RenderTarget& target, const std::string& text, float cx, float cy, unsigned int size, sf::Color color) {
    sf::Text t(text, getFont(), size);
    sf::FloatRect bounds = t.getLocalBounds();
    t.setOrigin(bounds.width / 2, bounds.height / 2 - bounds.height * 0.1f);
    t.setPosition(cx, cy);
    t.setFillColor(color);
    target.draw(t);
}

void Renderer::drawTextShadow(sf::RenderTarget& target, const std::string& text, float x, float y, unsigned int size, sf::Color color, sf::Color shadow, float offset) {
    sf::Text t(text, getFont(), size);
    t.setPosition(x + offset, y + offset);
    t.setFillColor(shadow);
    target.draw(t);
    t.setPosition(x, y);
    t.setFillColor(color);
    target.draw(t);
}

}
