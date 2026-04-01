# Snake Pro

A modern graphical Snake game built with C++17 and Raylib.

## Features

- Smooth 60 FPS graphical rendering
- Particle effects when eating food
- Pulsing food animation with glow
- Gradient snake body with animated eyes
- Grid-based board with glowing border
- Progressive difficulty system
- High score persistence
- Cross-platform (Windows, Linux, macOS)

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
| Arrow Keys / WASD | Move |
| P / ESC | Pause |
| R | Restart (Game Over) |
| Q / ESC | Quit |

## Build Status

[![Build](https://github.com/Zaid102071/Snake-Pro-C-Plus-Plus/actions/workflows/build.yml/badge.svg)](https://github.com/Zaid102071/Snake-Pro-C-Plus-Plus/actions/workflows/build.yml)
