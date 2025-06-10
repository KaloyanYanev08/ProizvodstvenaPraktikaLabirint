#include <stdio.h>
#include <unistd.h>
#include "pathFinding.h"
#include "UI.h"

CellConnection* getConnection(Node* node, Direction direction) {
    switch (direction) {
        case UP: return &node->up; break;
        case RIGHT: return &node->right; break;
        case DOWN: return &node->down; break;
        case LEFT: return &node->left; break;
        default: return NULL;
    }
}

void rightWallHugging(Node** maze, int row, int column) {
    int playerRow = 1;
    int playerCol = 1;
    Direction dir = RIGHT;

    int destRow = row - 2;
    int destCol = column - 2;

    for (int i = 0; i < row; i++)
        for (int j = 0; j < column; j++)
            maze[i][j].visited = 0;

    while (!(playerRow == destRow && playerCol == destCol)) {
        maze[playerRow][playerCol].visited = 1;

        clearScreen();
        drawMazeWithPath(maze, row, column, playerRow, playerCol);
        usleep(100000);

        Direction rightDir = (dir + 1) % 4;
        CellConnection* rightConnection = getConnection(&maze[playerRow][playerCol], rightDir);

        if (rightConnection && rightConnection->neighbor && rightConnection->wall->type == CELL) {
            dir = rightDir;
            switch(dir) {
                case UP: playerRow -= 2; break;
                case DOWN: playerRow += 2; break;
                case LEFT: playerCol -= 2; break;
                case RIGHT: playerCol += 2; break;
            }
        } else {
            CellConnection* frontConnection = getConnection(&maze[playerRow][playerCol], dir);
            if (frontConnection && frontConnection->neighbor && frontConnection->wall->type == CELL) {
                switch(dir) {
                    case UP: playerRow -= 2; break;
                    case DOWN: playerRow += 2; break;
                    case LEFT: playerCol -= 2; break;
                    case RIGHT: playerCol += 2; break;
                }
            } else {
                dir = (dir + 3) % 4;
            }
        }
    }

    maze[playerRow][playerCol].visited = 1;
    clearScreen();
    drawMazeWithPath(maze, row, column, playerRow, playerCol);
    printf("Path found!\n");
    sleep(2);
}

void dfsTraverse(Cursor cursor, Node* dest, int* found, Node** maze, int row, int column) {
    static int startRow = -1, startCol = -1;

    if (startRow == -1 && startCol == -1) {
        for (int i = 0; i < row; i++)
            for (int j = 0; j < column; j++)
                if (&maze[i][j] == cursor.position) {
                    startRow = i;
                    startCol = j;
                }
    }

    if (*found) return;

    cursor.position->visited = 1;
    int playerRow = -1, playerCol = -1;
    for (int i = 0; i < row; i++)
        for (int j = 0; j < column; j++)
            if (&maze[i][j] == cursor.position) {
                playerRow = i;
                playerCol = j;
                break;
            }

    clearScreen();
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            if (i == playerRow && j == playerCol) {
                printf("🟢 ");
            } else if (maze[i][j].type == WALL) {
                printf("⬜ ");
            } else if (maze[i][j].visited) {
                printf("⭕ ");
            } else {
                printf("⬛ ");
            }
        }
        printf("\n");
    }
    usleep(100000);

    if (cursor.position == dest) {
        *found = 1;
        printf("Path found!\n");
        sleep(2);
        return;
    }

    for (Direction dir = UP; dir <= LEFT && !(*found); dir++) {
        CellConnection* connection = getConnection(cursor.position, dir);
        if (connection && connection->neighbor && connection->wall->type == CELL && !connection->neighbor->visited) {
            Cursor newCursor = {connection->neighbor, dir};
            dfsTraverse(newCursor, dest, found, maze, row, column);
        }
    }
}