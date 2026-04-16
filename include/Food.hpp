#pragma once

#include "Snake.hpp"
#include <random>

namespace snake {

class Food {
public:
    Food();

    void spawn(const Snake& snake);
    void respawn(const Snake& snake);

    [[nodiscard]] Position position() const noexcept;

private:
    Position              pos_;
    std::mt19937          rng_;
};

}
