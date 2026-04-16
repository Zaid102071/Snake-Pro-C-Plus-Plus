#include "Snake.hpp"

namespace snake {

Snake::Snake(Position start)
    : dir_(Direction::Right), next_dir_(Direction::Right), growing_(false) {
    body_.push_back(start);
    body_.push_back({start.x - 1, start.y});
    body_.push_back({start.x - 2, start.y});
}

void Snake::set_direction(Direction dir) {
    if (dir == dir_) return;

    switch (dir_) {
        case Direction::Up:    if (dir == Direction::Down)    return; break;
        case Direction::Down:  if (dir == Direction::Up)      return; break;
        case Direction::Left:  if (dir == Direction::Right)   return; break;
        case Direction::Right: if (dir == Direction::Left)    return; break;
    }
    next_dir_ = dir;
}

void Snake::update() {
    dir_ = next_dir_;

    Position new_head = body_.front();

    switch (dir_) {
        case Direction::Up:    --new_head.y; break;
        case Direction::Down:  ++new_head.y; break;
        case Direction::Left:  --new_head.x; break;
        case Direction::Right: ++new_head.x; break;
    }

    if (new_head.x >= GRID_COLS) new_head.x = GRID_COLS - 1;
    if (new_head.y >= GRID_ROWS) new_head.y = GRID_ROWS - 1;

    body_.push_front(new_head);

    if (growing_) {
        growing_ = false;
    } else {
        body_.pop_back();
    }
}

void Snake::grow() {
    growing_ = true;
}

void Snake::reset(Position start) {
    body_.clear();
    dir_ = Direction::Right;
    next_dir_ = Direction::Right;
    growing_ = false;

    body_.push_back(start);
    body_.push_back({start.x - 1, start.y});
    body_.push_back({start.x - 2, start.y});
}

const std::deque<Position>& Snake::body() const noexcept {
    return body_;
}

Position Snake::head() const noexcept {
    return body_.front();
}

Direction Snake::direction() const noexcept {
    return dir_;
}

bool Snake::is_self_colliding() const {
    if (body_.size() < 2) return false;

    const Position& head = body_.front();
    for (auto it = body_.begin() + 1; it != body_.end(); ++it) {
        if (*it == head) return true;
    }
    return false;
}

Direction Snake::direction_from_delta(const Position& from, const Position& to) const {
    if (to.x > from.x) return Direction::Right;
    if (to.x < from.x) return Direction::Left;
    if (to.y > from.y) return Direction::Down;
    return Direction::Up;
}

}
