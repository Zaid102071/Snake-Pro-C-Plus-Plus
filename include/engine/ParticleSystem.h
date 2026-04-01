#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

namespace snakepro {

class ParticleSystem {
public:
    struct Particle {
        sf::Vector2f pos;
        sf::Vector2f vel;
        float life = 0;
        float maxLife = 0;
        float size = 0;
        sf::Color color;
    };

    void emit(sf::Vector2f pos, int count, sf::Color col, float speed = 100, float life = 0.6f, float size = 4);
    void emitBurst(sf::Vector2f pos, int count, sf::Color col, float speed = 100, float life = 0.6f, float size = 4);
    void update(float dt);
    void render(sf::RenderTarget& target);
    void clear();
    int count() const { return static_cast<int>(data_.size()); }

private:
    std::vector<Particle> data_;
};

}
