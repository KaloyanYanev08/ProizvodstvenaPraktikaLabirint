#ifndef MAZEGEN_H
#define MAZEGEN_H

typedef struct Node Node;
typedef struct CellConnection {
    Node* neighbor;
    Node* wall;
} CellConnection;

typedef struct Node {
    enum { WALL, CELL } type;
    int visited;
    CellConnection up, down, left, right;
} Node;

Node** init2dMaze(int row, int column);
void recursiveBacktrackingMazeGenerate(Node* current, unsigned int* seed);
unsigned int nextRandom(unsigned int* seed);
void shuffle(CellConnection** dirs, int count, unsigned int* seed);
void drawMaze(Node** maze, int row, int column);
void freeMaze(Node** maze, int row);

#endif