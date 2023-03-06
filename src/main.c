// main.c source code 
#include "../headers/main.h"


int main(void)
{
    maze_t maze;

    loadMaze(&maze);
    printMatrix(&(maze.grid), maze.row_count,maze.col_count);

    return 0;
}