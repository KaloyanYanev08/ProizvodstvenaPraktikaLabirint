#include <stdio.h>
#include <stdlib.h>

typedef struct Node Node;

typedef struct CellConnection{
    Node* neighbor;  // adjacent CELL node
    Node* wall;      // wall node between current and neighbor
} CellConnection;

typedef struct Node {
    enum { WALL, CELL } type;
    int visited;

    CellConnection up;
    CellConnection down;
    CellConnection left;
    CellConnection right;
} Node;


Node** init2dMaze(int column, int row) {
    Node** maze = malloc(column * sizeof(Node*));
    for (int i = 0; i < column; i++) {
        maze[i] = malloc(row * sizeof(Node));
        for (int j = 0; j < row; j++) {
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
    for (int i = 1; i < column; i += 2) {
        for (int j = 1; j < row; j += 2) {
            Node* current = &maze[i][j];

            if (i - 2 > 0 && maze[i - 2][j].type == CELL) {
                current->up.neighbor = &maze[i - 2][j];
                current->up.wall = &maze[i - 1][j];
            }

            if (i + 2 < column && maze[i + 2][j].type == CELL) {
                current->down.neighbor = &maze[i + 2][j];
                current->down.wall = &maze[i + 1][j];
            }

            if (j - 2 > 0 && maze[i][j - 2].type == CELL) {
                current->left.neighbor = &maze[i][j - 2];
                current->left.wall = &maze[i][j - 1];
            }

            if (j + 2 < row && maze[i][j + 2].type == CELL) {
                current->right.neighbor = &maze[i][j + 2];
                current->right.wall = &maze[i][j + 1];
            }
        }
    }

    return maze;
}

void drawMaze(Node** maze, int column, int row){
    for(int i = 0; i < column; i++){
        for(int j = 0; j < row; j++){
            if(maze[i][j].type == WALL){
                printf("⬜ ");
            } else {
                printf("⬛ ");
            }
        }
        printf("\n");
    }
}

void freeMaze(Node** maze, int column){
    for(int i = 0; i < column; i++){
        free(maze[i]);
    }
    free(maze);
}

int main(){
    
}