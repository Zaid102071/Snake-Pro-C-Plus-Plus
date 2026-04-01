# Snake Pro

A modern, beautifully-rendered Snake game built with C++17 and Raylib.

## Features

- **Stunning Visuals** - Gradient backgrounds, glowing borders, animated food with pulsing glow
- **Particle Effects** - Burst particles when eating food, death explosion on game over
- **Screen Shake** - Dynamic screen shake on food collection and death
- **Smooth Animations** - Pulsing snake head, gradient body that fades toward tail
- **High Score System** - Persistent top 10 scores with dates
- **Responsive Controls** - Arrow keys and WASD support
- **Cross-Platform** - Windows, Linux, macOS
- **Professional Architecture** - Clean separation of engine, game, and UI layers

## Screenshots

The game features a dark theme with neon green accents, glowing golden food, and smooth particle effects.

## Building

### Prerequisites

- CMake 3.20+
- C++17 compatible compiler
- Raylib (auto-downloaded via FetchContent)

### Build Commands

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --parallel
```

## Controls

| Key | Action |
|-----|--------|
| Arrow Keys / WASD | Move the snake |
| P / ESC | Pause / Resume |
| R | Restart (Game Over screen) |
| Q | Quit to menu |
| H | High Scores (Menu screen) |
| ENTER / SPACE | Start game |

## Project Structure

```
Snake-Pro-C-Plus-Plus/
├── include/
│   ├── core/           # Core types and settings
│   │   ├── Settings.h
│   │   ├── Types.h
│   │   └── Math.h
│   ├── engine/         # Rendering engine
│   │   ├── ColorPalette.h
│   │   ├── Renderer.h
│   │   └── ParticleSystem.h
│   └── game/           # Game logic
│       ├── Snake.h
│       ├── ScoreManager.h
│       └── Game.h
├── src/
│   ├── main.cpp
│   ├── engine/
│   └── game/
├── CMakeLists.txt
└── .github/workflows/
```

## Build Status

[![Build](https://github.com/Zaid102071/Snake-Pro-C-Plus-Plus/actions/workflows/build.yml/badge.svg)](https://github.com/Zaid102071/Snake-Pro-C-Plus-Plus/actions/workflows/build.yml)
