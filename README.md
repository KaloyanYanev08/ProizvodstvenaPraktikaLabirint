# Maze Generation in C

This is a C program that generates a maze and solves it using a pathfinding algorithm (e.g., right-wall hugging).
The user can try to solve the maze himself or run a pathfinding algorith and see how it works.
The maze can also be written in a file and read from a file.

---

## ⚙️ How to Compile

Ensure you have **GCC installed** (MinGW for Windows). Then, open a terminal in the project directory and run:

```bash
gcc maze.c mazeGen.c pathFinding.c UI.c -o maze.exe
.\maze.exe

