# Snake Pro

A clean Windows Snake game using pure Win32 API — zero external dependencies.

## Features

- **Zero dependencies** — Only Windows API, no DLLs needed
- **Single executable** — No external libraries, fonts, or resources
- **Gradient snake body** with animated eyes
- **Glowing food** with highlight effect
- **High score persistence**
- **Progressive difficulty**
- **Pause/Resume**

## Building

```bash
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

## Controls

| Key | Action |
|-----|--------|
| Arrow Keys / WASD | Move |
| P / ESC | Pause |
| R | Restart (Game Over) |
| Q | Quit to Menu |
| ENTER / SPACE | Start |
