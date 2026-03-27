# RUSH game

This project is a two-level game developed in C using the Raylib library as part of the Principles of Programming (POP) assignment.

The aim of this project was to demonstrate a strong understanding of C programming, including structured design, modular development, and algorithmic problem solving. The game combines real-time interaction with a generated maze, creating a structured but slightly different experience each time it is played.

---

## Build and Run

This project is designed to be built and run using the POP environment provided by the university.

### Build

Run the following command from the project root:

```bash
/opt/pop/bin/build-wasm.sh src/main.c
```

This generates an `out` directory containing the files needed to run the game in a browser.

---

### First-time setup

```bash
/opt/pop/bin/allocate_port.sh
echo $MY_PORT
```

---

### Run

```bash
/opt/pop/bin/run-wasm.sh
```

---

### Open in browser

Open:

```
localhost:XXXXX
```

Replace `XXXXX` with your allocated port.

---

## Game Overview

The game is divided into two levels, each designed to test different aspects of gameplay and logic.

### Level 1 – Catch the Marks

Two players compete on a split screen. Each player can only interact with items falling on their own side.

There are three item types:

* Green items: +5 marks
* Gold items: +10 marks (faster and smaller)
* Bombs: −10 marks

Each player has a maximum of 100 marks available. The marks collected in this level are carried into Level 2.

---

### Level 2 – The Maze

In the second level, each player navigates a maze and must:

* reach the exit
* have at least 40 marks

The level introduces time pressure and movement constraints, requiring both planning and efficient navigation.

---

## Algorithms and Technical Approach

### Maze Generation (Depth-First Search)

The maze is generated using a recursive Depth-First Search (DFS) algorithm. The grid starts as a set of walls, and paths are carved by exploring unvisited neighbours.

This guarantees:

* every cell is reachable
* there is exactly one path between any two points

This produces a valid and solvable maze every time while maintaining randomness.

---

### Maze Mirroring for Fairness

Instead of generating two independent mazes, the second maze is created by mirroring the first one horizontally.

This ensures:

* identical difficulty for both players
* consistent gameplay conditions
* fairness in competitive play

---

## Game Architecture

### State Management

The game flow is controlled using an enum-based state system. Each screen or phase of the game is represented by a state (title, story, gameplay, end screens).

This approach:

* keeps logic organised
* simplifies transitions between screens
* improves readability and debugging

---

### Modular Design

The project is structured into separate modules, each responsible for a specific part of the program:

* `shared.h`
  Contains global constants, structs, and enums

* `text.c / text.h`
  Handles text rendering utilities

* `level1.c / level1.h`
  Manages item spawning, scoring, and collision detection

* `maze.c / maze.h`
  Handles maze generation, movement logic, and rendering

This separation improves maintainability and reflects good software engineering practices.

---

### Player Representation

Each player is represented using a struct that stores both gameplay and state information:

```c
typedef struct {
    float x, y;
    int row, col;
    int marks;
    float moveCooldown;
    bool reachedGoal;
} Player;
```

This allows consistent handling of players across both levels and simplifies logic reuse.

---

### Collision Detection

Level 1 uses circle-to-rectangle collision detection to determine when a player collects an item. This method is efficient and suitable for real-time gameplay.

Movement and updates are based on frame time, ensuring consistent behaviour regardless of frame rate.

---

## Design Justification

Several design decisions were made to improve both gameplay and code quality.

* The DFS algorithm was chosen for maze generation because it guarantees a solvable maze with a clear structure while remaining efficient to implement in C.

* Maze mirroring was used to ensure fairness between players, avoiding differences in difficulty between the two sides.

* Enum-based state management provides a clear and scalable way to control game flow.

* Modular design improves readability and maintainability by separating different parts of the program.

* Using structs for players and systems allows better organisation of data and simplifies function interactions.

---

## Concepts Demonstrated

This project demonstrates:

* Use of structs and enums in C
* Pointer-based data manipulation
* Modular programming with multiple source files
* Real-time game loop design
* Procedural generation using DFS
* Fairness in game design
* Collision detection
* State-based architecture

---

## Project Structure

```
rush/
├── src/        main game logic
├── include/    header files
├── resources/  game assets
├── out/        build output
```

---

## Conclusion

This project was designed to go beyond basic requirements by focusing on structure, clarity, and algorithmic thinking. It demonstrates both functional gameplay and careful design decisions .
