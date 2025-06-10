#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include <time.h>
#include "UI.h"

void clearScreen() {
    printf("\033[2J\033[H");
    printf("\n\n"); //top line gets cut off for some reason so this is a poor attempt at fixing it
}

void printMenu() {
    sleep(1);
    clearScreen();
    printf("\n=== МЕНЮ ===\n");
    printf("1. Генериране на лабиринт\n");
    printf("2. Четене от файл\n");
    printf("3. Записване във файл\n");
    printf("4. Обхождане от потребителя\n");
    printf("5. Обхождане от компютъра\n");
    printf("6. Изход\n");
    printf("Избор: ");
}

void drawMazeWithPath(Node** maze, int row, int column, int playerRow, int playerCol) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            if (i == playerRow && j == playerCol) {
                printf("🟢 "); // Player
            } else if (maze[i][j].type == WALL) {
                printf("⬜ ");
            } else if (maze[i][j].visited) {
                printf("⭕ "); // Visited path
            } else {
                printf("⬛ "); // Unvisited path
            }
        }
        printf("\n");
    }
}

void userTraverse(Node** maze, int row, int column) {
    int playerRow = 1;
    int playerCol = 0; // входа

    int endRow = row - 2;
    int endCol = column - 1; // изхода

    // Зануляваме visited
    for (int i = 0; i < row; i++)
        for (int j = 0; j < column; j++)
            maze[i][j].visited = 0;

    maze[playerRow][playerCol].visited = 1;

    while (!(playerRow == endRow && playerCol == endCol)) {
        clearScreen();
        drawMazeWithPath(maze, row, column, playerRow, playerCol);
        printf("WASD / ZQSD / Arrow keys (←↑↓→) to move. Press 'X' to quit.\n");

        char move = getch();
        int newRow = playerRow;
        int newCol = playerCol;

        if (move == 0 || move == -32) {
            // Arrow key input
            char arrow = getch();
            switch (arrow) {
                case 72: newRow--; break; // Up
                case 80: newRow++; break; // Down
                case 75: newCol--; break; // Left
                case 77: newCol++; break; // Right
                default: continue;
            }
        } else {
            // AZERTY + QWERTY keys
            switch (move) {
                case 'w': case 'W': case 'z': case 'Z': newRow--; break; // Up
                case 's': case 'S': newRow++; break; // Down
                case 'a': case 'A': case 'q': case 'Q': newCol--; break; // Left
                case 'd': case 'D': newCol++; break; // Right
                case 'x': case 'X': return; // Quit
                default: continue;
            }
        }

        if (newRow >= 0 && newRow < row &&
            newCol >= 0 && newCol < column &&
            maze[newRow][newCol].type == CELL) {
            playerRow = newRow;
            playerCol = newCol;
            maze[playerRow][playerCol].visited = 1;
        }
    }

    drawMazeWithPath(maze, row, column, playerRow, playerCol);
    printf("Достигна края на лабиринта!\n");
}


void saveMazeToFile(Node** maze, int row, int column, unsigned int seed, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        exit(1);
        return;
    }

    fprintf(file, "%d %d\n%u\n", row, column, seed);

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            fputc(maze[i][j].type == WALL ? '#' : ' ', file);
        }
        fputc('\n', file);
    }

    fclose(file);
}

Node** loadMazeFromFile(const char* filename, int* outRow, int* outColumn, unsigned int* outSeed) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Cannot open file for reading");
        return NULL;
    }

    int row, column;
    unsigned int seed;
    fscanf(file, "%d %d\n%u\n", &row, &column, &seed);
    *outRow = row;
    *outColumn = column;
    *outSeed = seed;

    Node** maze = init2dMaze(row, column);

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            int ch = fgetc(file);
            if (ch == '#')
                maze[i][j].type = WALL;
            else if (ch == ' ')
                maze[i][j].type = CELL;
        }
        fgetc(file);
    }

    fclose(file);

    for (int i = 1; i < row; i += 2) {
        for (int j = 1; j < column; j += 2) {
            Node* current = &maze[i][j];

            if (i - 2 > 0 && maze[i - 2][j].type == CELL) {
                current->up.neighbor = &maze[i - 2][j];
                current->up.wall = &maze[i - 1][j];
            }

            if (i + 2 < row && maze[i + 2][j].type == CELL) {
                current->down.neighbor = &maze[i + 2][j];
                current->down.wall = &maze[i + 1][j];
            }

            if (j - 2 > 0 && maze[i][j - 2].type == CELL) {
                current->left.neighbor = &maze[i][j - 2];
                current->left.wall = &maze[i][j - 1];
            }

            if (j + 2 < column && maze[i][j + 2].type == CELL) {
                current->right.neighbor = &maze[i][j + 2];
                current->right.wall = &maze[i][j + 1];
            }
        }
    }

    return maze;
}