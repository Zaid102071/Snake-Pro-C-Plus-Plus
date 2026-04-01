#pragma once

#include <SFML/System/Vector2.hpp>
#include <cmath>

namespace snakepro {

using Vec2 = sf::Vector2f;
using IVec2 = sf::Vector2i;

inline float length(const Vec2& v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}

inline Vec2 normalize(const Vec2& v) {
    float l = length(v);
    return l > 0 ? v / l : Vec2{0, 0};
}

}
