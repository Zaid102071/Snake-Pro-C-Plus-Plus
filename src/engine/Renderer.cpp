#include "engine/Renderer.h"
#include <cmath>

namespace snakepro {

float Renderer::shakeAmount_ = 0;
float Renderer::shakeTimer_ = 0;

static Rectangle makeRect(float x, float y, float w, float h) {
    Rectangle r;
    r.x = x; r.y = y; r.width = w; r.height = h;
    return r;
}

void Renderer::drawRoundedRect(float x, float y, float w, float h, float roundness, Color col) {
    DrawRectangleRounded(makeRect(x, y, w, h), roundness, 8, col);
}

void Renderer::drawRoundedRectOutline(float x, float y, float w, float h, float roundness, float thickness, Color col) {
    DrawRectangleRoundedLines(makeRect(x, y, w, h), roundness, 8, thickness, col);
}

void Renderer::drawGlowCircle(float x, float y, float radius, Color core, Color glow, float glowRadius) {
    DrawCircle(x, y, glowRadius, glow);
    DrawCircle(x, y, radius, core);
    DrawCircle(x, y, radius * 0.5f, WHITE);
}

void Renderer::drawGradientRect(float x, float y, float w, float h, Color top, Color bottom) {
    DrawRectangleGradientV(x, y, w, h, top, bottom);
}

void Renderer::drawTextCentered(const char* text, float cx, float cy, int fontSize, Color col) {
    int tw = MeasureText(text, fontSize);
    DrawText(text, cx - tw / 2, cy - fontSize / 2, fontSize, col);
}

void Renderer::drawTextShadow(const char* text, float x, float y, int fontSize, Color col, Color shadow, float offset) {
    DrawText(text, x + offset, y + offset, fontSize, shadow);
    DrawText(text, x, y, fontSize, col);
}

void Renderer::setScreenShake(float amount) {
    shakeAmount_ = amount;
    shakeTimer_ = 0.15f;
}

float Renderer::getScreenShake() {
    return shakeAmount_;
}

void Renderer::updateScreenShake(float dt) {
    if (shakeTimer_ > 0) {
        shakeTimer_ -= dt;
        if (shakeTimer_ <= 0) {
            shakeAmount_ = 0;
        }
    }
}

void Renderer::drawScreenShake(float amount) {
    if (amount > 0.5f) {
        float sx = (static_cast<float>(rand() % 200) / 100.f - 1.f) * amount;
        float sy = (static_cast<float>(rand() % 200) / 100.f - 1.f) * amount;
        BeginBlendMode(BLEND_ALPHA);
        EndBlendMode();
        rlTranslatef(sx, sy, 0);
    }
}

}
