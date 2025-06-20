# Maze Generation in C

This is a C program that generates a maze and solves it using a pathfinding algorithm (e.g., right-wall hugging).
The user can try to solve the maze himself or run a pathfinding algorith and see how it works.
The maze can also be written in a file and read from a file.

---

## ⚙️ How to Compile

Ensure you have **GCC installed** (MinGW for Windows). Then, open a terminal in the project directory and run:

```bash
gcc maze.c mazeGen.c pathFinding.c UI.c -o maze.exe
If you are using CMD run the commands:
chcp 65001
maze.exe
If you are using PowerShell run the commands:
chcp 65001
$OutputEncoding = [Console]::OutputEncoding = [Text.UTF8Encoding]::new()
.\maze.exe
```
The two commands after you make the maze.exe file are for forcing the CMD and Powershell to use the UTF-8 encoding so that the maze displays as it should.

