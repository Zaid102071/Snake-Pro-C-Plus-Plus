#pragma once

#include <vector>
#include <deque>
#include "Constants.hpp"

namespace snake {

enum class Direction { Up, Down, Left, Right };

struct Position {
    std::uint32_t x;
    std::uint32_t y;

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
    bool operator!=(const Position& other) const {
        return !(*this == other);
    }
};

class Snake {
public:
    explicit Snake(Position start);

    void set_direction(Direction dir);
    void update();
    void grow();
    void reset(Position start);

    [[nodiscard]] const std::deque<Position>& body() const noexcept;
    [[nodiscard]] Position head() const noexcept;
    [[nodiscard]] Direction direction() const noexcept;
    [[nodiscard]] bool is_self_colliding() const;

private:
    Direction direction_from_delta(const Position& from, const Position& to) const;

    std::deque<Position> body_;
    Direction            dir_;
    Direction            next_dir_;
    bool                 growing_;
};

}
