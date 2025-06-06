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

void clearScreen() {
    printf("\033[2J\033[H");
    printf("\n\n"); //top line gets cut off for some reason so this is a poor attempt at fixing it
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


Node** maze = NULL;
unsigned int seed = 12345;

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

Node** loadMazeFromFile(const char* filename, int* outRow, int* outColumn) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Cannot open file for reading");
        return NULL;
    }

    int row, column;
    fscanf(file, "%d %d\n", &row, &column);
    *outRow = row;
    *outColumn = column;

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

int main() {
    int choice;
    int row = 0;
    int column = 0;

    while (1) {
        printMenu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                if (maze != NULL) {
                    freeMaze(maze, row);
                }

                do {
                    printf("Въведете брой редове (нечетно число): ");
                    scanf("%d", &row);
                } while (!(row % 2));

                do {
                    printf("Въведете брой колони (нечетно число): ");
                    scanf("%d", &column);
                } while (!(column % 2));

                maze = init2dMaze(row, column);
                maze[1][0].type = CELL;                   // вход
                maze[row - 2][column - 1].type = CELL;     // изход
                recursiveBacktrackingMazeGenerate(&maze[1][1], &seed);
                drawMaze(maze, row, column);
                break;

            case 2:
                if (maze != NULL) {
                    freeMaze(maze, row);
                }
                maze = loadMazeFromFile("maze.txt", &row, &column);
                if (maze) {
                    printf("Лабиринтът е зареден успешно.\n");
                    drawMaze(maze, row, column);
                }
                break;

            case 3:
                if (maze == NULL) {
                    printf("Няма лабиринт за запис.\n");
                } else {
                    saveMazeToFile(maze, row, column, "maze.txt");
                    printf("Лабиринтът е записан успешно.\n");
                }
                break;

            case 4:
                if (maze == NULL) {
                    printf("Първо генерирай лабиринт.\n");
                } else {
                    userTraverse(maze, row, column);
                }
                break;
            case 5:
                if (maze == NULL) {
                    printf("Първо генерирай лабиринт.\n");
                    break;
                }

                // Reset visited flags before traversal
                for (int i = 0; i < row; i++)
                    for (int j = 0; j < column; j++)
                        maze[i][j].visited = 0;

                printf("Изберете метод за обхождане:\n");
                printf("1. DFS (Дълбочинно първо търсене)\n");
                printf("2. Обхождане с дясна стена (Right-Wall Hugging)\n");
                printf("Ваш избор: ");

                int traverseChoice;
                scanf("%d", &traverseChoice);

                // Define start and destination nodes
                Node* start = &maze[1][1];
                Node* dest = &maze[row - 2][column - 2];

                switch (traverseChoice) {
                    case 1: {
                        int found = 0;
                        Cursor startCursor = {start, UP};
                        dfsTraverse(startCursor, dest, &found, maze, row, column);
                        if (!found) {
                            printf("Пътят не е намерен.\n");
                            sleep(2);
                        }
                        break;
                    }
                    case 2:
                        rightWallHugging(maze, row, column);
                        break;
                    default:
                        printf("Невалиден избор.\n");
                        sleep(2);
                        break;
                }
                break;
            case 6:
                if (maze != NULL) {
                    freeMaze(maze, row);
                }
                printf("Изход...\n");
                return 0;

            default:
                printf("Невалиден избор. Опитайте отново.\n");
                break;
        }
    }
}