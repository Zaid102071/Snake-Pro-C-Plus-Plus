#pragma once

#include "game/Direction.h"

namespace snakepro {

class InputHandler {
public:
    InputHandler();
    ~InputHandler();
    
    Direction pollInput();
    static void setupTerminal();
    static void restoreTerminal();
    
private:
    bool running_;
};

}
