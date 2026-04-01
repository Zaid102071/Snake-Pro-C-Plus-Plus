# Snake Pro

A modern, beautifully-rendered Snake game built with **pure C++17** and **SFML**.

## Features

- **Pure C++17** - Native C++ library (SFML), no C wrappers
- **Stunning Visuals** - Gradient backgrounds, glowing borders, animated food with pulsing glow
- **Particle Effects** - Burst particles when eating food, death explosion on game over
- **Screen Shake** - Dynamic screen shake on food collection and death
- **Smooth Animations** - Pulsing snake head, gradient body that fades toward tail
- **High Score System** - Persistent top 10 scores with dates
- **Responsive Controls** - Arrow keys and WASD support with direction locking
- **Cross-Platform** - Windows, Linux, macOS
- **Professional Architecture** - Clean separation of core, engine, and game layers

## Tech Stack

| Component | Technology |
|-----------|------------|
| Language | C++17 |
| Graphics | SFML 3.0 (native C++) |
| Build | CMake 3.20+ with FetchContent |
| CI/CD | GitHub Actions (multi-platform) |

## Building

### Prerequisites

- CMake 3.20+
- C++17 compatible compiler
- SFML 3.0 (auto-downloaded via FetchContent)

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
| Q / ESC | Quit to menu |
| H | High Scores (Menu screen) |
| ENTER / SPACE | Start game |

## Project Structure

```
Snake-Pro-C-Plus-Plus/
├── include/
│   ├── core/           # Core types and configuration
│   │   ├── Config.h    # Game constants
│   │   ├── Types.h     # Enums and state
│   │   └── Math.h      # Vector utilities
│   ├── engine/         # Rendering engine
│   │   ├── Colors.h    # Centralized color palette
│   │   ├── Renderer.h  # Text and shape helpers
│   │   └── ParticleSystem.h  # Particle effects
│   └── game/           # Game logic
│       ├── Game.h      # Main game class
│       ├── Snake.h     # Snake and Food classes
│       └── ScoreManager.h  # High score persistence
├── src/
│   ├── main.cpp
│   ├── engine/
│   └── game/
├── CMakeLists.txt
└── .github/workflows/build.yml
```

## Build Status

[![Build](https://github.com/Zaid102071/Snake-Pro-C-Plus-Plus/actions/workflows/build.yml/badge.svg)](https://github.com/Zaid102071/Snake-Pro-C-Plus-Plus/actions/workflows/build.yml)
