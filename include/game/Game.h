#pragma once

#include "game/Snake.h"
#include "game/Renderer.h"
#include "game/InputHandler.h"
#include "game/ScoreManager.h"
#include "game/Config.h"

namespace snakepro {

class Game {
public:
    Game();
    ~Game();
    
    void run();
    
private:
    void init();
    void gameLoop();
    void handleInput();
    void update();
    void render();
    void resetGame();
    
    int getSpeed() const;
    int getLevel() const;
    int getRawKey();
    
    Snake snake_;
    Food food_;
    Renderer renderer_;
    InputHandler input_;
    ScoreManager scoreManager_;
    
    int score_;
    bool running_;
    bool paused_;
    bool gameOver_;
    int width_;
    int height_;
};

}
