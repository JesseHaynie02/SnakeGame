# Snake Game in C

## Table of Contents

- [Features](#features)
- [Installation](#installation)

## Features

- Classic Snake game implemented in C.
- User-controlled snake movement.
- Game board rendering using SDL2.
- Collision detection with walls and itself.
- Score tracking and display.

## Installation

### Option 1: Using Makefile (Recommended)

1. Clone the repository: `git clone https://github.com/your-username/snake-game-c.git`
2. Navigate to the project directory.
3. Execute the `make` command.
    ```bash
    make
    ```
4. Run the executable.
    ```bash
    ./snake
    ```

### Option 2: Manual Compilation

1. Clone the repository: `git clone https://github.com/your-username/snake-game-c.git`
2. Navigate to the project directory.
3. Compile the code using GCC with necessary flags.
    ```bash
    gcc -Iinclude -o snake.exe src/snake.c -Llib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf
    ```
4. Run the executable.
    ```bash
    ./snake
    ```
    
## Notes

- This program is designed to work on Windows systems.
