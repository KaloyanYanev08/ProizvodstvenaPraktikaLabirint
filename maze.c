#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "mazeGen.h"
#include "pathFinding.h"
#include "UI.h"

Node** maze = NULL;

int main() {
    int choice;
    int row = 0;
    int column = 0;
    unsigned int seed;

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

                char conformation = '\0';
                do {
                    printf("Искате ли да въведете специфичен seed?\nАко не искате ще се генерира случаен такъв (y-да,n-не): ");
                    scanf(" %c",&conformation);
                } while (conformation != 'y' && conformation !='Y' && conformation != 'n' && conformation != 'N');

                if (conformation == 'y' || conformation == 'Y') {
                    printf("Въведете стойността на seed: ");
                    scanf("%u", &seed);
                } else {
                    seed = (unsigned int)time(NULL);
                }
                printf("\n");

                maze = init2dMaze(row, column);
                maze[1][0].type = CELL;                   // вход
                maze[row - 2][column - 1].type = CELL;     // изход
                recursiveBacktrackingMazeGenerate(&maze[1][1], &seed);
                drawMaze(maze, row, column);
                printf("\nРазмери: %dx%d", column, row);
                printf("\nSeed: %u",seed);
                break;

            case 2:
                if (maze != NULL) {
                    freeMaze(maze, row);
                }
                maze = loadMazeFromFile("maze.txt", &row, &column, &seed);
                if (maze) {
                    printf("Лабиринтът е зареден успешно.\n");
                    drawMaze(maze, row, column);
                }
                break;

            case 3:
                if (maze == NULL) {
                    printf("Няма лабиринт за запис.\n");
                } else {
                    saveMazeToFile(maze, row, column, seed, "maze.txt");
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