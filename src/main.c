// main.c source code
#include "../headers/main.h"


int main(void)
{
   


    maze_t maze;
    loadMaze(&maze);
    printMatrix(&(maze.grid), maze.row_count, maze.col_count);
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

    


    checkpoint_t start_checkpoint;
    start_checkpoint.current_track_record = create_node_list(maze.entry[0] * maze.col_count + maze.entry[1]);
    start_checkpoint.direction = (-maze.col_count);
    start_checkpoint.end_reached = false;
    start_checkpoint.p_maze = &maze;
    start_checkpoint.last_pos = maze.entry[0] * maze.col_count + maze.entry[1];

    start_checkpoint.limited_threads = true;
    start_checkpoint.free_threads_count = malloc(sizeof(sem_t));
    sem_init(start_checkpoint.free_threads_count, 0, 10);

    int used_threads = 0;
    start_checkpoint.used_threads_count = &used_threads;

    solveMaze_threaded(&start_checkpoint);
    printf("used_threads_count = %d\n", *(start_checkpoint.used_threads_count));


    if (getLength(start_checkpoint.current_track_record) >= 1)
    {
        print_list(start_checkpoint.current_track_record);
        start_checkpoint.current_track_record = destroy_list(start_checkpoint.current_track_record);
    }

    destroyMaze(&maze);
    sem_destroy(start_checkpoint.free_threads_count);
  
    
    return 0;
}