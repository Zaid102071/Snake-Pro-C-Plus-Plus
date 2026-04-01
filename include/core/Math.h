#pragma once

namespace snakepro {

struct Vec2 {
    float x = 0;
    float y = 0;

    Vec2 operator+(const Vec2& o) const { return {x + o.x, y + o.y}; }
    Vec2 operator-(const Vec2& o) const { return {x - o.x, y - o.y}; }
    Vec2 operator*(float s) const { return {x * s, y * s}; }
    Vec2 operator/(float s) const { return {x / s, y / s}; }
    Vec2& operator+=(const Vec2& o) { x += o.x; y += o.y; return *this; }
    Vec2& operator-=(const Vec2& o) { x -= o.x; y -= o.y; return *this; }
    Vec2& operator*=(float s) { x *= s; y *= s; return *this; }
    float length() const { return sqrtf(x * x + y * y); }
    Vec2 normalized() const { float l = length(); return l > 0 ? *this / l : Vec2{0, 0}; }
};

inline bool operator==(const Vec2& a, const Vec2& b) {
    return a.x == b.x && a.y == b.y;
}

struct IVec2 {
    int x = 0;
    int y = 0;

    bool operator==(const IVec2& o) const { return x == o.x && y == o.y; }
    bool operator!=(const IVec2& o) const { return !(*this == o); }
};

}
