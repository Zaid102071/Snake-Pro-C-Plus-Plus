#pragma once

#include <raylib.h>

namespace snakepro {

class ColorPalette {
public:
    static Color bgDark()       { return {12, 12, 20, 255}; }
    static Color bgMid()        { return {18, 18, 30, 255}; }
    static Color bgLight()      { return {25, 25, 42, 255}; }
    static Color accent()       { return {0, 220, 160, 255}; }
    static Color accentBright() { return {0, 255, 190, 255}; }
    static Color accentDim()    { return {0, 160, 120, 255}; }
    static Color food()         { return {255, 210, 50, 255}; }
    static Color foodGlow()     { return {255, 180, 0, 80}; }
    static Color snakeHead()    { return {0, 255, 160, 255}; }
    static Color snakeBody(float t);
    static Color white()        { return {240, 240, 245, 255}; }
    static Color gray()         { return {140, 140, 160, 255}; }
    static Color dimGray()      { return {80, 80, 100, 255}; }
    static Color danger()       { return {255, 70, 70, 255}; }
    static Color dangerBright() { return {255, 100, 100, 255}; }
    static Color gold()         { return {255, 200, 50, 255}; }
    static Color overlay()      { return {0, 0, 0, 180}; }
    static Color gridLine()     { return {35, 35, 55, 100}; }
    static Color border()       { return {0, 200, 150, 255}; }
};

}
