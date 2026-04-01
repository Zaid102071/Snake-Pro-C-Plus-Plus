#include "engine/ParticleSystem.h"
#include <cmath>
#include <cstdlib>

namespace snakepro {

void ParticleSystem::emit(sf::Vector2f pos, int count, sf::Color col, float speed, float life, float size) {
    for (int i = 0; i < count; ++i) {
        Particle p;
        p.pos = pos;
        float angle = static_cast<float>(i) / static_cast<float>(count) * 6.28318f;
        float spd = speed * (0.5f + static_cast<float>(rand() % 100) / 100.f);
        p.vel = sf::Vector2f(std::cos(angle) * spd, std::sin(angle) * spd);
        p.life = life * (0.5f + static_cast<float>(rand() % 100) / 100.f);
        p.maxLife = p.life;
        p.size = size * (0.5f + static_cast<float>(rand() % 100) / 100.f);
        p.color = col;
        data_.push_back(p);
    }
}

void ParticleSystem::emitBurst(sf::Vector2f pos, int count, sf::Color col, float speed, float life, float size) {
    for (int i = 0; i < count; ++i) {
        Particle p;
        p.pos = pos;
        float angle = static_cast<float>(rand() % 360) * 0.0174533f;
        float spd = speed * (0.3f + static_cast<float>(rand() % 100) / 100.f);
        p.vel = sf::Vector2f(std::cos(angle) * spd, std::sin(angle) * spd);
        p.life = life * (0.4f + static_cast<float>(rand() % 100) / 100.f);
        p.maxLife = p.life;
        p.size = size * (0.4f + static_cast<float>(rand() % 100) / 100.f);
        p.color = col;
        data_.push_back(p);
    }
}

void ParticleSystem::update(float dt) {
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

void ParticleSystem::render(sf::RenderTarget& target) {
    for (const auto& p : data_) {
        float alpha = p.life / p.maxLife;
        sf::Color c = p.color;
        c.a = static_cast<sf::Uint8>(alpha * 255.f);
        sf::CircleShape shape(p.size * alpha);
        shape.setOrigin(p.size * alpha, p.size * alpha);
        shape.setPosition(p.pos);
        shape.setFillColor(c);
        target.draw(shape);
    }
}

void ParticleSystem::clear() {
    data_.clear();
}

}
