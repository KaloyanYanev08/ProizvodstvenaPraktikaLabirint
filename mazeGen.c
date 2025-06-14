#include <stdio.h>
#include <stdlib.h>
#include "mazeGen.h"

Node** init2dMaze(int row, int column) {
    Node** maze = malloc(row * sizeof(Node*));
    for (int i = 0; i < row; i++) {
        maze[i] = malloc(column * sizeof(Node));
        for (int j = 0; j < column; j++) {
            if (i % 2 == 1 && j % 2 == 1) {
                maze[i][j].type = CELL;
            } else {
                maze[i][j].type = WALL;
            }

            
            
            maze[i][j].visited = 0;

            // Initialize all connections to NULL
            maze[i][j].up.neighbor = NULL;
            maze[i][j].down.neighbor = NULL;
            maze[i][j].left.neighbor = NULL;
            maze[i][j].right.neighbor = NULL;
        }
    }

    // Set cell-to-cell connections and wall references
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

unsigned int nextRandom(unsigned int* seed) {
    *seed = *seed * 1103515245 + 12345;
    return *seed;
}

void shuffle(CellConnection** dirs, int count, unsigned int* seed) {
    for (int i = count - 1; i > 0; i--) {
        int j = nextRandom(seed) % (i + 1);
        CellConnection* temp = dirs[i];
        dirs[i] = dirs[j];
        dirs[j] = temp;
    }
}

void recursiveBacktrackingMazeGenerate(Node* current, unsigned int* seed) {
    current->visited = 1;

    // Create a list of pointers to connection structs
    CellConnection* directions[4] = {
        &current->up,
        &current->down,
        &current->left,
        &current->right
    };

    // Randomization
    shuffle(directions, 4, seed);

    // Explore unvisited neighbors
    for (int i = 0; i < 4; i++) {
        CellConnection* conn = directions[i];
        if (conn->neighbor && !conn->neighbor->visited) {
            conn->wall->type = CELL;           // Make the wall a walkable cell
            recursiveBacktrackingMazeGenerate(conn->neighbor, seed);
        }
    }
}

void drawMaze(Node** maze, int row, int column){
    for(int i = 0; i < row; i++){
        for(int j = 0; j < column; j++){
            if(maze[i][j].type == WALL){
                printf("⬜ ");
            } else {
                printf("⬛ ");
            }
        }
        printf("\n");
    }
}

void freeMaze(Node** maze, int row){
    for(int i = 0; i < row; i++){
        free(maze[i]);
    }
    free(maze);
}