#pragma once

#include "core/Math.h"
#include <raylib.h>

namespace snakepro {

class Renderer {
public:
    static void drawRoundedRect(float x, float y, float w, float h, float roundness, Color col);
    static void drawRoundedRectOutline(float x, float y, float w, float h, float roundness, float thickness, Color col);
    static void drawGlowCircle(float x, float y, float radius, Color core, Color glow, float glowRadius);
    static void drawGradientRect(float x, float y, float w, float h, Color top, Color bottom);
    static void drawTextCentered(const char* text, float cx, float cy, int fontSize, Color col);
    static void drawTextShadow(const char* text, float x, float y, int fontSize, Color col, Color shadow, float offset = 2);
    static void drawScreenShake(float amount);
    static void setScreenShake(float amount);
    static float getScreenShake();
    static void updateScreenShake(float dt);

private:
    static float shakeAmount_;
    static float shakeTimer_;
};

}
