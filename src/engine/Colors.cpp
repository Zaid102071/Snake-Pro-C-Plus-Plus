#include "engine/Colors.h"

namespace snakepro {

sf::Color Colors::snakeBody(float t) {
    sf::Uint8 g = static_cast<sf::Uint8>(150 + 105 * t);
    sf::Uint8 b = static_cast<sf::Uint8>(50 + 90 * t);
    return {0, g, b, 255};
}

}
