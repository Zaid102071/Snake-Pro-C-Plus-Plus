# Snake Pro - C++ Terminal Game

A modern, cross-platform Snake game written in C++17.

## Platform Support

| Platform | Status |
|----------|--------|
| Windows  | ✅     |
| Linux    | ✅     |
| macOS    | ✅     |

## Quick Start

### Windows
```powershell
mkdir build; cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release
.\Release\snake-pro.exe
```

### Linux/macOS
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --parallel
./snake-pro
```

## Controls

- **Arrow Keys** or **WASD** - Move the snake
- **P** - Pause/Resume
- **R** - Restart (after game over)
- **Q** or **ESC** - Quit

## Features

- Progressive difficulty (speed increases with score)
- High score persistence
- Color-coded rendering (green head, cyan body, yellow food)
- Cross-platform terminal input handling
- Clean OOP architecture

## Build Status

[![Build](https://github.com/Zaid102071/Snake-Pro-C-Plus-Plus/actions/workflows/build.yml/badge.svg)](https://github.com/Zaid102071/Snake-Pro-C-Plus-Plus/actions/workflows/build.yml)
