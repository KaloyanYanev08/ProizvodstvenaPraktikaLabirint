#include <stdio.h>
#include <stdlib.h>

int** init2dMaze(int row,int column){
    int** maze = (int**)malloc(row * sizeof(int*));
    for(int i = 0; i < row; i++){
        maze[i] = (int*)malloc(column * sizeof(int));
    }
    for(int i = 0; i < row; i++){
        for(int j = 0; j < column; j++){
            maze[i][j] = 1; // Initialize all cells to walls
        } //0 is empty, 1 is a wall
    }
    return maze;

}
void drawMaze(int** maze, int row, int column){
    for(int i = 0; i < row; i++){
        for(int j = 0; j < column; j++){
            if(maze[i][j] == 1){
                printf("#");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}
int main(){
    
}