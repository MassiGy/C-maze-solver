// main.c source code 
#include "../headers/main.h"


int main(void)
{
    maze_t maze;

    loadMaze(&maze);
    printMatrix(&(maze.grid), maze.row_count,maze.col_count);

    /* set the right coordinates for the entry/start*/
    maze.entry[0] = 3;
    maze.entry[1] = 1;
    /* set the right coordinates for the escape/end*/
    maze.end[0] = 3;
    maze.end[1] = 3;

    printf("maze.row_count = %d\n", maze.row_count);
    printf("maze.col_count = %d\n", maze.col_count);

    list_t* list = create_node_list(maze.entry[0] * maze.col_count + maze.entry[1]);

    solveMaze_rec(&maze,&list,3,1);

    print_list(list);

    list = destroy_list(list);

    destroyMaze(&maze);

    return 0;
}