# Snake Pro

A professional, industry-level Snake game built with C++ and SFML.

## Features

- Modern game loop with delta time
- Score system with high score persistence
- Multiple game states (Menu, Playing, Paused, GameOver)
- Gradient snake rendering with eyes
- Pulsing food animation
- Keyboard controls (WASD/Arrows)

## Build

### Linux/macOS
```bash
cmake -B build
cmake --build build
./build/snake-pro
```

### Windows
Requires SFML. Use vcpkg or download from sfml-dev.org.

## Controls

- WASD / Arrow Keys: Move
- P: Pause
- ESC: Menu/Quit
- Enter: Start/Restart
