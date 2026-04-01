#include "Snake.h"
#include <algorithm>

namespace snakepro {

Snake::Snake(int startX, int startY)
    : direction_(Direction::Right), nextDirection_(Direction::Right) {
    body_.emplace_back(Position{startX, startY});
    body_.emplace_back(Position{startX - 1, startY});
    body_.emplace_back(Position{startX - 2, startY});
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
        case Direction::Up:    newHead = {head.x, head.y - 1}; break;
        case Direction::Down:  newHead = {head.x, head.y + 1}; break;
        case Direction::Left:  newHead = {head.x - 1, head.y}; break;
        case Direction::Right: newHead = {head.x + 1, head.y}; break;
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

bool Snake::checkWallCollision(int gridWidth, int gridHeight) const {
    Position head = getHead();
    return head.x < 0 || head.x >= gridWidth ||
           head.y < 0 || head.y >= gridHeight;
}

Food::Food() : position_{0, 0}, rng_(std::random_device{}()) {}

void Food::spawn(int gridWidth, int gridHeight, const std::vector<Position>& snakeBody) {
    std::uniform_int_distribution<int> distX(0, gridWidth - 1);
    std::uniform_int_distribution<int> distY(0, gridHeight - 1);

    do {
        position_ = Position{distX(rng_), distY(rng_)};
    } while (std::find(snakeBody.begin(), snakeBody.end(), position_) != snakeBody.end());
}

}
