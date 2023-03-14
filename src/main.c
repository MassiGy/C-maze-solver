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


    list_t* list = create_node_list(maze.entry[0] * maze.col_count + maze.entry[1]);

    solveMaze_rec(&maze,&list,3,1);

    list = shift_list(list);


    print_list(list);

    list = destroy_list(list);

    destroyMaze(&maze);

    return 0;
}