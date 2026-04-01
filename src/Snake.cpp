#include "game/Snake.h"
#include <algorithm>

namespace snakepro {

Snake::Snake(int startX, int startY)
    : direction_(Direction::Right), nextDirection_(Direction::Right) {
    body_.emplace_back(startX, startY);
    body_.emplace_back(startX - 1, startY);
    body_.emplace_back(startX - 2, startY);
}

void Snake::grow() {
    Position tail = body_.back();
    body_.push_back(tail);
}

void Snake::move() {
    direction_ = nextDirection_;

    Position head = getHead();
    Position newHead;

    switch (direction_) {
        case Direction::Up:    newHead = {head.first, head.second - 1}; break;
        case Direction::Down:  newHead = {head.first, head.second + 1}; break;
        case Direction::Left:  newHead = {head.first - 1, head.second}; break;
        case Direction::Right: newHead = {head.first + 1, head.second}; break;
        default:               return;
    }

    body_.insert(body_.begin(), newHead);
    body_.pop_back();
}

void Snake::setDirection(Direction dir) {
    if (dir != Direction::None && dir != opposite(direction_)) {
        nextDirection_ = dir;
    }
}

bool Snake::checkSelfCollision() const {
    Position head = getHead();
    for (size_t i = 1; i < body_.size(); ++i) {
        if (body_[i] == head) {
            return true;
        }
    }
    return false;
}

bool Snake::checkWallCollision(int width, int height) const {
    Position head = getHead();
    return head.first < 0 || head.first >= width ||
           head.second < 0 || head.second >= height;
}

Food::Food() : position_(0, 0), rng_(std::random_device{}()) {}

void Food::spawn(int width, int height, const std::vector<Position>& snakeBody) {
    std::uniform_int_distribution<int> distX(0, width - 1);
    std::uniform_int_distribution<int> distY(0, height - 1);

    do {
        position_ = {distX(rng_), distY(rng_)};
    } while (std::find(snakeBody.begin(), snakeBody.end(), position_) != snakeBody.end());
}

}
