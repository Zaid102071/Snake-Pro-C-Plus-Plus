#include "engine/ColorPalette.h"

namespace snakepro {

Color ColorPalette::snakeBody(float t) {
    unsigned char g = static_cast<unsigned char>(160 + 95 * t);
    unsigned char b = static_cast<unsigned char>(60 + 80 * t);
    return {0, g, b, 255};
}

}
