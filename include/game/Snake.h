#pragma once

#include "core/Math.h"
#include "core/Types.h"
#include <vector>

namespace snakepro {

class Snake {
public:
    Snake(int startX, int startY);

    void grow(int amount = 1);
    void move();
    void setDirection(Direction dir);
    Direction getDirection() const { return direction_; }

    const std::vector<IVec2>& getBody() const { return body_; }
    IVec2 getHead() const { return body_.empty() ? IVec2{0, 0} : body_.front(); }
    int getLength() const { return static_cast<int>(body_.size()); }

    bool checkSelfCollision() const;
    bool checkWallCollision(int gridW, int gridH) const;

private:
    std::vector<IVec2> body_;
    Direction direction_ = Direction::Right;
    Direction nextDirection_ = Direction::Right;
};

class Food {
public:
    Food();

    void spawn(int gridW, int gridH, const std::vector<IVec2>& snakeBody);
    IVec2 getPosition() const { return position_; }

private:
    IVec2 position_;
};

}
