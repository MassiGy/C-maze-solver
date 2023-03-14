// main.c source code 
#include "../headers/main.h"


int main(void)
{
    maze_t maze;

    loadMaze(&maze);
    printMatrix(&(maze.grid), maze.row_count,maze.col_count);
    findStart(&maze);
    findEnd(&maze);

    printf("Entrée : ");
    for (int i = 0; i < 2; i++)
    {
        printf("[%d]", maze.entry[i]);
    }

    printf("\nSortie : ");
    for (int i = 0; i < 2; i++)
    {
        printf("[%d]", maze.end[i]);
    }
    

    return 0;
}