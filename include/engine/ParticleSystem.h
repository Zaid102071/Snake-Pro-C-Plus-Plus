#pragma once

#include "core/Math.h"
#include <raylib.h>
#include <vector>

namespace snakepro {

class Particle {
public:
    struct Data {
        Vec2 pos;
        Vec2 vel;
        float life = 0;
        float maxLife = 0;
        float size = 0;
        Color color;
    };

    void emit(Vec2 pos, int count, Color col, float speed = 100, float life = 0.6f, float size = 4);
    void emitBurst(Vec2 pos, int count, Color col, float speed = 100, float life = 0.6f, float size = 4);
    void update(float dt);
    void render();
    void clear();
    int count() const { return static_cast<int>(data_.size()); }

private:
    std::vector<Data> data_;
};

}
