// main.c source code 
#include "../headers/main.h"


int main(void)
{

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
    
    printf("\n");


    

    list_t* list = create_node_list(maze.entry[0] * maze.col_count + maze.entry[1]);

    solveMaze_rec(&maze,&list,maze.entry[0],maze.entry[1]);

    list = shift_list(list);

    if (getLength(list) >= 1)
    {
        print_list(list);
        list = destroy_list(list);
    }


    destroyMaze(&maze);

    return 0;
}