#include "game/Snake.h"
#include <algorithm>
#include <random>

namespace snakepro {

Snake::Snake(int startX, int startY) {
    body_.push_back(IVec2{startX, startY});
    body_.push_back(IVec2{startX - 1, startY});
    body_.push_back(IVec2{startX - 2, startY});
}

void Snake::grow(int amount) {
    for (int i = 0; i < amount; ++i) {
        body_.push_back(body_.back());
    }
}

void Snake::move() {
    direction_ = nextDirection_;

    IVec2 head = getHead();
    IVec2 newHead;

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
    IVec2 head = getHead();
    for (size_t i = 1; i < body_.size(); ++i) {
        if (body_[i] == head) return true;
    }
    return false;
}

bool Snake::checkWallCollision(int gridW, int gridH) const {
    IVec2 head = getHead();
    return head.x < 0 || head.x >= gridW || head.y < 0 || head.y >= gridH;
}

Food::Food() {}

void Food::spawn(int gridW, int gridH, const std::vector<IVec2>& snakeBody) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distX(0, gridW - 1);
    std::uniform_int_distribution<int> distY(0, gridH - 1);

    do {
        position_ = IVec2{distX(gen), distY(gen)};
    } while (std::find(snakeBody.begin(), snakeBody.end(), position_) != snakeBody.end());
}

}
