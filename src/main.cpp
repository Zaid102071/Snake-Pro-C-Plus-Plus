#include <windows.h>
#include <vector>
#include <random>
#include <string>
#include <ctime>

const int WIDTH = 800;
const int HEIGHT = 600;
const int GRID = 20;
const int CELL = 25;
const int BOARD_X = (WIDTH - GRID * CELL) / 2;
const int BOARD_Y = 80;
const int SCORE_PER_FOOD = 10;

enum Dir { UP, DOWN, LEFT, RIGHT };

struct Point { int x, y; };

std::vector<Point> snake;
Point food;
Dir dir = RIGHT;
Dir nextDir = RIGHT;
int score = 0;
int highScore = 0;
bool gameOver = false;
bool paused = false;
bool started = false;
int speed = 120;
HBRUSH hBrushBg, hBrushBoard, hBrushGrid, hBrushSnakeHead, hBrushSnakeBody, hBrushFood, hBrushFoodGlow, hBrushBorder, hBrushText;
HPEN hPenBorder;
HWND hwnd;
UINT_PTR timerId = 1;

void loadHighScore() {
    FILE* f = fopen("highscore.dat", "rb");
    if (f) { fread(&highScore, sizeof(highScore), 1, f); fclose(f); }
}

void saveHighScore() {
    FILE* f = fopen("highscore.dat", "wb");
    if (f) { fwrite(&highScore, sizeof(highScore), 1, f); fclose(f); }
}

void spawnFood() {
    std::mt19937 rng(static_cast<unsigned>(time(nullptr)));
    std::uniform_int_distribution<int> dx(0, GRID - 1);
    std::uniform_int_distribution<int> dy(0, GRID - 1);
    do {
        food = { dx(rng), dy(rng) };
    } while (std::find_if(snake.begin(), snake.end(), [&](const Point& p) {
        return p.x == food.x && p.y == food.y;
    }) != snake.end());
}

void resetGame() {
    snake.clear();
    snake.push_back({ GRID / 2, GRID / 2 });
    snake.push_back({ GRID / 2 - 1, GRID / 2 });
    snake.push_back({ GRID / 2 - 2, GRID / 2 });
    dir = RIGHT;
    nextDir = RIGHT;
    score = 0;
    gameOver = false;
    paused = false;
    speed = 120;
    spawnFood();
    SetTimer(hwnd, timerId, speed, nullptr);
}

void update() {
    if (gameOver || paused) return;

    dir = nextDir;
    Point head = snake.front();
    switch (dir) {
        case UP:    head.y--; break;
        case DOWN:  head.y++; break;
        case LEFT:  head.x--; break;
        case RIGHT: head.x++; break;
    }

    if (head.x < 0 || head.x >= GRID || head.y < 0 || head.y >= GRID) {
        gameOver = true;
        if (score > highScore) { highScore = score; saveHighScore(); }
        KillTimer(hwnd, timerId);
        return;
    }

    for (size_t i = 0; i < snake.size(); i++) {
        if (snake[i].x == head.x && snake[i].y == head.y) {
            gameOver = true;
            if (score > highScore) { highScore = score; saveHighScore(); }
            KillTimer(hwnd, timerId);
            return;
        }
    }

    snake.insert(snake.begin(), head);

    if (head.x == food.x && head.y == food.y) {
        score += SCORE_PER_FOOD;
        spawnFood();
        if (speed > 50) {
            speed -= 3;
            KillTimer(hwnd, timerId);
            SetTimer(hwnd, timerId, speed, nullptr);
        }
    } else {
        snake.pop_back();
    }
}

void drawText(HDC hdc, const char* text, int x, int y, COLORREF color, int size = 16) {
    HFONT hFont = CreateFontA(size, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                              OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                              DEFAULT_PITCH | FF_DONTCARE, "Segoe UI");
    HFONT hOld = (HFONT)SelectObject(hdc, hFont);
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, color);
    TextOutA(hdc, x, y, text, (int)strlen(text));
    SelectObject(hdc, hOld);
    DeleteObject(hFont);
}

void drawTextCentered(HDC hdc, const char* text, int y, COLORREF color, int size = 16) {
    HFONT hFont = CreateFontA(size, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                              OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                              DEFAULT_PITCH | FF_DONTCARE, "Segoe UI");
    HFONT hOld = (HFONT)SelectObject(hdc, hFont);
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, color);
    SIZE sz;
    GetTextExtentPoint32A(hdc, text, (int)strlen(text), &sz);
    TextOutA(hdc, (WIDTH - sz.cx) / 2, y, text, (int)strlen(text));
    SelectObject(hdc, hOld);
    DeleteObject(hFont);
}

void render(HDC hdc) {
    // Background
    HBRUSH hBg = CreateSolidBrush(RGB(15, 15, 25));
    SelectObject(hdc, hBg);
    Rectangle(hdc, 0, 0, WIDTH, HEIGHT);
    DeleteObject(hBg);

    if (!started) {
        drawTextCentered(hdc, "SNAKE PRO", HEIGHT / 2 - 80, RGB(0, 230, 170), 48);
        drawTextCentered(hdc, "A Modern Snake Game", HEIGHT / 2 - 30, RGB(140, 140, 160), 18);
        drawTextCentered(hdc, "Press ENTER or SPACE to Start", HEIGHT / 2 + 20, RGB(220, 220, 230), 20);
        char buf[64];
        sprintf_s(buf, "High Score: %d", highScore);
        drawTextCentered(hdc, buf, HEIGHT / 2 + 70, RGB(255, 205, 50), 18);
        drawTextCentered(hdc, "Arrow Keys / WASD - Move", HEIGHT / 2 + 130, RGB(80, 80, 100), 14);
        drawTextCentered(hdc, "P - Pause  |  Q - Quit", HEIGHT / 2 + 155, RGB(80, 80, 100), 14);
        return;
    }

    // Board border
    int bw = GRID * CELL;
    int bh = GRID * CELL;
    HBRUSH hBorder = CreateSolidBrush(RGB(0, 210, 160));
    SelectObject(hdc, hBorder);
    Rectangle(hdc, BOARD_X - 4, BOARD_Y - 4, BOARD_X + bw + 4, BOARD_Y + bh + 4);
    DeleteObject(hBorder);

    // Board background
    HBRUSH hBoard = CreateSolidBrush(RGB(10, 10, 18));
    SelectObject(hdc, hBoard);
    Rectangle(hdc, BOARD_X, BOARD_Y, BOARD_X + bw, BOARD_Y + bh);
    DeleteObject(hBoard);

    // Grid lines
    HPEN hGridPen = CreatePen(PS_SOLID, 1, RGB(30, 30, 50));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hGridPen);
    for (int x = 0; x <= GRID; x++) {
        MoveToEx(hdc, BOARD_X + x * CELL, BOARD_Y, nullptr);
        LineTo(hdc, BOARD_X + x * CELL, BOARD_Y + bh);
    }
    for (int y = 0; y <= GRID; y++) {
        MoveToEx(hdc, BOARD_X, BOARD_Y + y * CELL, nullptr);
        LineTo(hdc, BOARD_X + bw, BOARD_Y + y * CELL);
    }
    SelectObject(hdc, hOldPen);
    DeleteObject(hGridPen);

    // Food glow
    int fx = BOARD_X + food.x * CELL + CELL / 2;
    int fy = BOARD_Y + food.y * CELL + CELL / 2;
    for (int r = CELL; r > 0; r -= 2) {
        int alpha = r * 3;
        if (alpha > 60) alpha = 60;
        HBRUSH hGlow = CreateSolidBrush(RGB(255, 200, 50));
        SelectObject(hdc, hGlow);
        SetDCBrushColor(hdc, RGB(255, 200, 50));
        SetDCPenColor(hdc, RGB(255, 200, 50));
        Ellipse(hdc, fx - r, fy - r, fx + r, fy + r);
        DeleteObject(hGlow);
    }

    // Food
    HBRUSH hFood = CreateSolidBrush(RGB(255, 215, 60));
    SelectObject(hdc, hFood);
    Ellipse(hdc, fx - CELL / 2 + 3, fy - CELL / 2 + 3, fx + CELL / 2 - 3, fy + CELL / 2 - 3);
    DeleteObject(hFood);

    // Food highlight
    HBRUSH hHighlight = CreateSolidBrush(RGB(255, 255, 220));
    SelectObject(hdc, hHighlight);
    Ellipse(hdc, fx - 4, fy - 4, fx + 4, fy + 4);
    DeleteObject(hHighlight);

    // Snake body
    for (int i = (int)snake.size() - 1; i >= 0; i--) {
        int sx = BOARD_X + snake[i].x * CELL;
        int sy = BOARD_Y + snake[i].y * CELL;
        float t = 1.0f - (float)i / (float)snake.size();
        int g = (int)(150 + 105 * t);
        int b = (int)(50 + 90 * t);

        if (i == 0) {
            HBRUSH hHead = CreateSolidBrush(RGB(0, 255, 170));
            SelectObject(hdc, hHead);
            Rectangle(hdc, sx + 1, sy + 1, sx + CELL - 1, sy + CELL - 1);
            DeleteObject(hHead);

            HBRUSH hInner = CreateSolidBrush(RGB(100, 255, 220));
            SelectObject(hdc, hInner);
            Rectangle(hdc, sx + 3, sy + 3, sx + CELL - 3, sy + CELL - 3);
            DeleteObject(hInner);

            // Eyes
            int ex1, ey1, ex2, ey2;
            int cx = sx + CELL / 2;
            int cy = sy + CELL / 2;
            int off = CELL / 4;
            switch (dir) {
                case RIGHT: ex1 = cx + off; ey1 = cy - off; ex2 = cx + off; ey2 = cy + off; break;
                case LEFT:  ex1 = cx - off; ey1 = cy - off; ex2 = cx - off; ey2 = cy + off; break;
                case UP:    ex1 = cx - off; ey1 = cy - off; ex2 = cx + off; ey2 = cy - off; break;
                case DOWN:  ex1 = cx - off; ey1 = cy + off; ex2 = cx + off; ey2 = cy + off; break;
                default:    ex1 = cx; ey1 = cy; ex2 = cx; ey2 = cy; break;
            }
            HBRUSH hWhite = CreateSolidBrush(RGB(255, 255, 255));
            SelectObject(hdc, hWhite);
            Ellipse(hdc, ex1 - 3, ey1 - 3, ex1 + 3, ey1 + 3);
            Ellipse(hdc, ex2 - 3, ey2 - 3, ex2 + 3, ey2 + 3);
            DeleteObject(hWhite);

            HBRUSH hBlack = CreateSolidBrush(RGB(0, 0, 0));
            SelectObject(hdc, hBlack);
            Ellipse(hdc, ex1 - 1, ey1 - 1, ex1 + 1, ey1 + 1);
            Ellipse(hdc, ex2 - 1, ey2 - 1, ex2 + 1, ey2 + 1);
            DeleteObject(hBlack);
        } else {
            HBRUSH hBody = CreateSolidBrush(RGB(0, (BYTE)g, (BYTE)b));
            SelectObject(hdc, hBody);
            Rectangle(hdc, sx + 2, sy + 2, sx + CELL - 2, sy + CELL - 2);
            DeleteObject(hBody);
        }
    }

    // HUD
    char hud[128];
    sprintf_s(hud, "SCORE: %d", score);
    drawText(hdc, hud, BOARD_X, BOARD_Y + bh + 15, RGB(0, 230, 170), 18);

    sprintf_s(hud, "HIGH: %d", highScore);
    SIZE sz;
    HFONT hTmp = CreateFontA(18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                             OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                             DEFAULT_PITCH | FF_DONTCARE, "Segoe UI");
    HDC hdcTmp = hdc;
    SelectObject(hdcTmp, hTmp);
    GetTextExtentPoint32A(hdcTmp, hud, (int)strlen(hud), &sz);
    DeleteObject(hTmp);
    drawText(hdc, hud, BOARD_X + bw / 2 - sz.cx / 2, BOARD_Y + bh + 15, RGB(255, 205, 50), 18);

    int level = score / SCORE_PER_FOOD / 5 + 1;
    sprintf_s(hud, "LEVEL: %d", level);
    drawText(hdc, hud, BOARD_X + bw - 80, BOARD_Y + bh + 15, RGB(140, 140, 160), 18);

    drawTextCentered(hdc, "P: Pause  |  Q: Menu  |  Arrows/WASD: Move", HEIGHT - 25, RGB(80, 80, 100), 12);

    if (paused) {
        HBRUSH hOverlay = CreateSolidBrush(RGB(0, 0, 0));
        SelectObject(hdc, hOverlay);
        SetBkMode(hdc, TRANSPARENT);
        RECT rc = { 0, 0, WIDTH, HEIGHT };
        FillRect(hdc, &rc, hOverlay);
        DeleteObject(hOverlay);
        drawTextCentered(hdc, "PAUSED", HEIGHT / 2 - 30, RGB(0, 230, 170), 48);
        drawTextCentered(hdc, "Press P to Resume", HEIGHT / 2 + 20, RGB(140, 140, 160), 18);
    }

    if (gameOver) {
        HBRUSH hOverlay = CreateSolidBrush(RGB(0, 0, 0));
        SelectObject(hdc, hOverlay);
        SetBkMode(hdc, TRANSPARENT);
        RECT rc = { 0, 0, WIDTH, HEIGHT };
        FillRect(hdc, &rc, hOverlay);
        DeleteObject(hOverlay);
        drawTextCentered(hdc, "GAME OVER", HEIGHT / 2 - 80, RGB(255, 70, 80), 48);
        sprintf_s(hud, "SCORE: %d", score);
        drawTextCentered(hdc, hud, HEIGHT / 2 - 20, RGB(235, 235, 245), 28);
        sprintf_s(hud, "BEST: %d", highScore);
        drawTextCentered(hdc, hud, HEIGHT / 2 + 20, RGB(255, 205, 50), 20);
        if (score >= highScore && score > 0) {
            drawTextCentered(hdc, "NEW HIGH SCORE!", HEIGHT / 2 + 60, RGB(255, 200, 50), 16);
        }
        drawTextCentered(hdc, "R - Play Again", HEIGHT / 2 + 110, RGB(0, 230, 170), 18);
        drawTextCentered(hdc, "Q - Main Menu", HEIGHT / 2 + 140, RGB(140, 140, 160), 16);
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            render(hdc);
            EndPaint(hWnd, &ps);
            return 0;
        }
        case WM_KEYDOWN: {
            int key = (int)wParam;
            if (!started) {
                if (key == VK_RETURN || key == VK_SPACE) {
                    started = true;
                    resetGame();
                    InvalidateRect(hWnd, nullptr, TRUE);
                }
                return 0;
            }
            if (gameOver) {
                if (key == 'R') { resetGame(); InvalidateRect(hWnd, nullptr, TRUE); }
                else if (key == 'Q' || key == VK_ESCAPE) { started = false; InvalidateRect(hWnd, nullptr, TRUE); }
                return 0;
            }
            if (paused) {
                if (key == 'P' || key == VK_ESCAPE) { paused = false; InvalidateRect(hWnd, nullptr, TRUE); }
                else if (key == 'Q') { started = false; KillTimer(hWnd, timerId); InvalidateRect(hWnd, nullptr, TRUE); }
                return 0;
            }
            if (key == 'P' || key == VK_ESCAPE) { paused = true; InvalidateRect(hWnd, nullptr, TRUE); return 0; }
            if (key == 'Q') { started = false; KillTimer(hWnd, timerId); InvalidateRect(hWnd, nullptr, TRUE); return 0; }
            if ((key == VK_UP || key == 'W') && dir != DOWN) nextDir = UP;
            if ((key == VK_DOWN || key == 'S') && dir != UP) nextDir = DOWN;
            if ((key == VK_LEFT || key == 'A') && dir != RIGHT) nextDir = LEFT;
            if ((key == VK_RIGHT || key == 'D') && dir != LEFT) nextDir = RIGHT;
            return 0;
        }
        case WM_TIMER: {
            update();
            InvalidateRect(hWnd, nullptr, FALSE);
            return 0;
        }
        case WM_DESTROY: {
            KillTimer(hWnd, timerId);
            PostQuitMessage(0);
            return 0;
        }
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmd, int nCmd) {
    loadHighScore();

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = "SnakePro";
    RegisterClass(&wc);

    hwnd = CreateWindow("SnakePro", "SNAKE PRO", WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
                        CW_USEDEFAULT, CW_USEDEFAULT, WIDTH, HEIGHT, nullptr, nullptr, hInst, nullptr);

    ShowWindow(hwnd, nCmd);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}
