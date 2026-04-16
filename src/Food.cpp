#include "Food.hpp"

namespace snake {

Food::Food() : pos_{0, 0}, rng_(std::random_device{}()) {}

void Food::spawn(const Snake& snake) {
    respawn(snake);
}

void Food::respawn(const Snake& snake) {
    std::uniform_int_distribution<std::uint32_t> dist_x(1, GRID_COLS - 2);
    std::uniform_int_distribution<std::uint32_t> dist_y(1, GRID_ROWS - 2);

    bool valid = false;
    while (!valid) {
        pos_.x = dist_x(rng_);
        pos_.y = dist_y(rng_);
        valid = true;

        for (const auto& seg : snake.body()) {
            if (seg == pos_) {
                valid = false;
                break;
            }
        }
    }
}

Position Food::position() const noexcept {
    return pos_;
}

}
