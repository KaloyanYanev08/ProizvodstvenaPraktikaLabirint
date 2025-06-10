#ifndef UI_H
#define UI_H

#include "mazeGen.h"

void clearScreen();
void printMenu();
void drawMazeWithPath(Node** maze, int row, int column, int playerRow, int playerCol);
void userTraverse(Node** maze, int row, int column);
void saveMazeToFile(Node** maze, int row, int column, unsigned int seed, const char* filename);
Node** loadMazeFromFile(const char* filename, int* outRow, int* outColumn, unsigned int* outSeed);

#endif