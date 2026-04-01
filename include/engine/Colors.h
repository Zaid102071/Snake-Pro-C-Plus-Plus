#pragma once

#include <SFML/Graphics/Color.hpp>

namespace snakepro {

struct Colors {
    static sf::Color bg()          { return {13, 13, 22, 255}; }
    static sf::Color bgLight()     { return {20, 20, 35, 255}; }
    static sf::Color board()       { return {10, 10, 18, 255}; }
    static sf::Color accent()      { return {0, 230, 170, 255}; }
    static sf::Color accentBright(){ return {0, 255, 200, 255}; }
    static sf::Color accentDim()   { return {0, 180, 130, 255}; }
    static sf::Color food()        { return {255, 215, 60, 255}; }
    static sf::Color foodGlow()    { return {255, 200, 50, 60}; }
    static sf::Color snakeHead()   { return {0, 255, 170, 255}; }
    static sf::Color snakeBody(float t);
    static sf::Color text()        { return {235, 235, 245, 255}; }
    static sf::Color textDim()     { return {140, 140, 160, 255}; }
    static sf::Color textMuted()   { return {80, 80, 100, 255}; }
    static sf::Color danger()      { return {255, 70, 80, 255}; }
    static sf::Color gold()        { return {255, 205, 50, 255}; }
    static sf::Color overlay()     { return {0, 0, 0, 185}; }
    static sf::Color grid()        { return {30, 30, 50, 90}; }
    static sf::Color border()      { return {0, 210, 160, 255}; }
    static sf::Color shadow()      { return {0, 0, 0, 80}; }
};

}
