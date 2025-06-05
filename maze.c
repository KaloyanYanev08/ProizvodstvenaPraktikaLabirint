#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>

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

void drawMazeWithPath(Node** maze, int row, int column){
    for(int i = 0; i < row; i++){
        for(int j = 0; j < column; j++){
            if(maze[i][j].type == WALL){
                printf("⬜ ");
            } else if (maze[i][j].visited) {
                printf("⭕ "); // червен квадрат (⬛)
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

typedef enum {
    UP=0,
    RIGHT,
    DOWN,
    LEFT
}Direction;

typedef struct{
    Node* position;
    Direction direction;
} Cursor;

CellConnection* getConnection(Node* node, Direction direction) {
    switch (direction) {
        case UP: return &node->up; break;
        case RIGHT: return &node->right; break;
        case DOWN: return &node->down; break;
        case LEFT: return &node->left; break;
        default: return NULL;
    }
}

void rightWallHugging(Node* start, Node* dest){
    Cursor cursor;
    cursor.position = start;
    cursor.direction = RIGHT;

    while(cursor.position != dest){
        CellConnection* connection = getConnection(cursor.position, cursor.direction);
        CellConnection* connectionRight = getConnection(cursor.position, (cursor.direction + 1) % 4);

        if (connectionRight && connectionRight->neighbor && connectionRight->wall->type == CELL) {
            cursor.position = connectionRight->neighbor;
            cursor.direction = (cursor.direction + 1) % 4;
        } else if (connection && connection->neighbor && connection->wall->type == CELL) {
            cursor.position = connection->neighbor;
        } else {
            cursor.direction = (cursor.direction + 1) % 4;
        }
    }
}

void dfsTraverseHelper(Cursor cursor, Node* dest, int* found) {
    if (*found) return;

    cursor.position->visited = 1;
    if (cursor.position == dest) {
        *found = 1;
        return;
    }

    for (Direction dir = UP; dir <= LEFT; dir++) {
        CellConnection* connection = getConnection(cursor.position, dir);
        if (connection && connection->neighbor && connection->wall->type == CELL && connection->neighbor->visited == 0) {
            Cursor newCursor = cursor;
            newCursor.position = connection->neighbor;
            newCursor.direction = dir;
            dfsTraverseHelper(newCursor, dest, found);
        }
    }
}

void dfsTraverse(Node* start, Node* dest){
    int found = 0;
    Cursor cursor;
    cursor.position = start;
    cursor.direction = UP;
    dfsTraverseHelper(cursor, dest, &found);
}

Node** maze = NULL;
unsigned int seed = 12345;

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

void userTraverse(Node** maze, int row, int column) {
    int playerRow = 1;
    int playerCol = 0; // входа

    int endRow = row - 2;
    int endCol = column - 1; // изхода

    char move;

    // Зануляваме visited
    for (int i = 0; i < row; i++)
        for (int j = 0; j < column; j++)
            maze[i][j].visited = 0;

    maze[playerRow][playerCol].visited = 1;

    while (!(playerRow == endRow && playerCol == endCol)) {
        clearScreen();
        drawMazeWithPath(maze, row, column);
        printf("W/A/S/D + Enter: ");
        //scanf(" %c", &move);
        move = getch();

        int newRow = playerRow;
        int newCol = playerCol;

        if (move == 'w' || move == 'W') newRow--;
        else if (move == 's' || move == 'S') newRow++;
        else if (move == 'a' || move == 'A') newCol--;
        else if (move == 'd' || move == 'D') newCol++;
        else if (move == 'q') break;
        else continue;

        if (newRow >= 0 && newRow < row &&
            newCol >= 0 && newCol < column &&
            maze[newRow][newCol].type == CELL) {
            playerRow = newRow;
            playerCol = newCol;
            maze[playerRow][playerCol].visited = 1;
        }

        printf("\n");
    }

    drawMazeWithPath(maze, row, column);
    printf("Достигна края на лабиринта!\n");
}

void saveMazeToFile(Node** maze, int row, int column, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        exit(1);
        return;
    }

    fprintf(file, "%d %d\n", row, column);

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            fputc(maze[i][j].type == WALL ? '#' : ' ', file);
        }
        fputc('\n', file);
    }

    fclose(file);
}


int main(){
    int choice;
    int row=0;
    int column = 0;
    while (1) {
        printMenu();
        scanf("%d", &choice);

        if(choice == 1) {
            if(maze != NULL) {
                freeMaze(maze, row);
            }
            do{
            printf("Въведете брой редове (нечетно число): ");
            scanf("%d", &row);
            }while(!(row%2));
            do{
            printf("Въведете брой колони (нечетно число): ");
            scanf("%d", &column);
            }while(!(column%2));

            maze = init2dMaze(row, column);
            Node* start = &maze[1][1];
            maze[1][0].type = CELL;
            maze[row-2][column-1].type = CELL;
            recursiveBacktrackingMazeGenerate(start, &seed);
            drawMaze(maze, row, column);

        }
        if (choice == 4) {
    if (maze == NULL) {
        printf("Първо генерирай лабиринт.\n");
    } else {
        userTraverse(maze, row, column);
    }
}
        if(choice==6) break;
    }
}