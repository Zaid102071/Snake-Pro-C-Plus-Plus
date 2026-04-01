#include "engine/ParticleSystem.h"
#include <cmath>
#include <cstdlib>

namespace snakepro {

void Particle::emit(Vec2 pos, int count, Color col, float speed, float life, float size) {
    for (int i = 0; i < count; ++i) {
        Data d;
        d.pos = pos;
        float angle = static_cast<float>(i) / static_cast<float>(count) * 6.28318f;
        float spd = speed * (0.5f + static_cast<float>(rand() % 100) / 100.f);
        d.vel = Vec2{cosf(angle) * spd, sinf(angle) * spd};
        d.life = life * (0.5f + static_cast<float>(rand() % 100) / 100.f);
        d.maxLife = d.life;
        d.size = size * (0.5f + static_cast<float>(rand() % 100) / 100.f);
        d.color = col;
        data_.push_back(d);
    }
}

void Particle::emitBurst(Vec2 pos, int count, Color col, float speed, float life, float size) {
    for (int i = 0; i < count; ++i) {
        Data d;
        d.pos = pos;
        float angle = static_cast<float>(rand() % 360) * 0.0174533f;
        float spd = speed * (0.3f + static_cast<float>(rand() % 100) / 100.f);
        d.vel = Vec2{cosf(angle) * spd, sinf(angle) * spd};
        d.life = life * (0.4f + static_cast<float>(rand() % 100) / 100.f);
        d.maxLife = d.life;
        d.size = size * (0.4f + static_cast<float>(rand() % 100) / 100.f);
        d.color = col;
        data_.push_back(d);
    }
}

void Particle::update(float dt) {
    for (auto it = data_.begin(); it != data_.end();) {
        it->pos += it->vel * dt;
        it->vel *= 0.97f;
        it->life -= dt;
        if (it->life <= 0) {
            it = data_.erase(it);
        } else {
            ++it;
        }
    }
}

void Particle::render() {
    for (const auto& d : data_) {
        float alpha = d.life / d.maxLife;
        unsigned char a = static_cast<unsigned char>(alpha * 255.f);
        Color c = d.color;
        c.a = a;
        DrawCircle(d.pos.x, d.pos.y, d.size * alpha, c);
    }
}

void Particle::clear() {
    data_.clear();
}

}
