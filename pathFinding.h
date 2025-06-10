#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "mazeGen.h"

typedef enum {
    UP=0,
    RIGHT,
    DOWN,
    LEFT
}Direction;

typedef struct {
    Node* position;
    Direction direction;
} Cursor;

CellConnection* getConnection(Node* node, Direction direction);
void rightWallHugging(Node** maze, int row, int column);
void dfsTraverse(Cursor cursor, Node* dest, int* found, Node** maze, int row, int column);

#endif