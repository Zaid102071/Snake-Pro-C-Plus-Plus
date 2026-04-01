#pragma once

#include "Direction.h"
#include "Position.h"
#include <vector>
#include <random>

namespace snakepro {

class Snake {
public:
    Snake(int startX, int startY);

    void grow();
    void move();
    void setDirection(Direction dir);
    Direction getDirection() const { return direction_; }

    const std::vector<Position>& getBody() const { return body_; }
    Position getHead() const { return body_.front(); }

    bool checkSelfCollision() const;
    bool checkWallCollision(int gridWidth, int gridHeight) const;

private:
    std::vector<Position> body_;
    Direction direction_;
    Direction nextDirection_;
};

class Food {
public:
    Food();

    void spawn(int gridWidth, int gridHeight, const std::vector<Position>& snakeBody);
    Position getPosition() const { return position_; }

private:
    Position position_;
    std::mt19937 rng_;
};

}
