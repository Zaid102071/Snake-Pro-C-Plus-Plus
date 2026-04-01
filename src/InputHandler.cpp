#include "game/InputHandler.h"
#include "game/Direction.h"

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#endif

namespace snakepro {

#ifdef _WIN32
static DWORD g_oldMode;
static bool g_modeSaved = false;
#else
static struct termios g_oldTermios;
static bool g_termiosSaved = false;
#endif

void InputHandler::setupTerminal() {
#ifdef _WIN32
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    g_modeSaved = GetConsoleMode(hIn, &g_oldMode) != 0;
    if (g_modeSaved) {
        SetConsoleMode(hIn, g_oldMode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT));
    }
#else
    struct termios newTermios;
    if (tcgetattr(STDIN_FILENO, &g_oldTermios) == 0) {
        newTermios = g_oldTermios;
        newTermios.c_lflag &= ~(ICANON | ECHO);
        newTermios.c_cc[VMIN] = 0;
        newTermios.c_cc[VTIME] = 0;
        tcsetattr(STDIN_FILENO, TCSANOW, &newTermios);
        g_termiosSaved = true;
    }
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
#endif
}

void InputHandler::restoreTerminal() {
#ifdef _WIN32
    if (g_modeSaved) {
        SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), g_oldMode);
        g_modeSaved = false;
    }
#else
    if (g_termiosSaved) {
        tcsetattr(STDIN_FILENO, TCSANOW, &g_oldTermios);
        g_termiosSaved = false;
    }
#endif
}

Direction InputHandler::pollInput() {
#ifdef _WIN32
    if (!_kbhit()) {
        return Direction::None;
    }

    int key = _getch();

    if (key == 0 || key == 224) {
        key = _getch();
        switch (key) {
            case 72: return Direction::Up;
            case 80: return Direction::Down;
            case 75: return Direction::Left;
            case 77: return Direction::Right;
        }
        return Direction::None;
    }

    switch (key) {
        case 'w': case 'W': return Direction::Up;
        case 's': case 'S': return Direction::Down;
        case 'a': case 'A': return Direction::Left;
        case 'd': case 'D': return Direction::Right;
        case 'p': case 'P': return Direction::None;
        case 'q': case 'Q': return Direction::None;
        case 27:            return Direction::None;
        default:            return Direction::None;
    }
#else
    char c;
    if (read(STDIN_FILENO, &c, 1) <= 0) {
        return Direction::None;
    }

    if (c == 27) {
        char seq[2];
        if (read(STDIN_FILENO, &seq[0], 1) == 1 && seq[0] == '[') {
            if (read(STDIN_FILENO, &seq[1], 1) == 1) {
                switch (seq[1]) {
                    case 'A': return Direction::Up;
                    case 'B': return Direction::Down;
                    case 'C': return Direction::Right;
                    case 'D': return Direction::Left;
                }
            }
        }
        return Direction::None;
    }

    switch (c) {
        case 'w': case 'W': return Direction::Up;
        case 's': case 'S': return Direction::Down;
        case 'a': case 'A': return Direction::Left;
        case 'd': case 'D': return Direction::Right;
        case 'p': case 'P': return Direction::None;
        case 'q': case 'Q': return Direction::None;
        default:            return Direction::None;
    }
#endif
}

InputHandler::InputHandler() {
    setupTerminal();
}

InputHandler::~InputHandler() {
    restoreTerminal();
}

}
