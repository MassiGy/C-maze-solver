// main.c source code
#include "../headers/main.h"

int main(void)
{

    maze_t maze;
    loadMaze(&maze);
    printMatrix(&(maze.grid), maze.row_count, maze.col_count);
    findStartNEndNTarget(&maze);

    printf("Entrée 1 : ");
    for (int i = 0; i < 2; i++)
    {
        printf("[%d]", maze.fstEntry[i]);
    }

    printf("\nSortie 1 : ");
    for (int i = 0; i < 2; i++)
    {
        printf("[%d]", maze.fstEnd[i]);
    }

    printf("Entrée 2 : ");
    for (int i = 0; i < 2; i++)
    {
        printf("[%d]", maze.sndEntry[i]);
    }

    printf("\nSortie 2 : ");
    for (int i = 0; i < 2; i++)
    {
        printf("[%d]", maze.sndEnd[i]);
    }

    printf("\nCible : ");
    for (int i = 0; i < 2; i++)
    {
        printf("[%d]", maze.target[i]);
    }

    printf("\n");

    checkpoint_t start_checkpoint;
    start_checkpoint.current_track_record = create_node_list(maze.fstEntry[0] * maze.col_count + maze.fstEntry[1]);
    start_checkpoint.direction = -(maze.col_count);
    start_checkpoint.end_reached = false;
    start_checkpoint.p_maze = &maze;
    start_checkpoint.last_pos = maze.fstEntry[0] * maze.col_count + maze.fstEntry[1];

    start_checkpoint.limited_threads = true;
    start_checkpoint.lock = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(start_checkpoint.lock, NULL);

    int limit = 2;
    start_checkpoint.p_free_threads_count = &limit;

    

    solveMaze_threaded(&start_checkpoint);


    if (getLength(start_checkpoint.current_track_record) >= 1)
    {
        print_list(start_checkpoint.current_track_record);
        start_checkpoint.current_track_record = destroy_list(start_checkpoint.current_track_record);
    }

    destroyMaze(&maze);
    pthread_mutex_destroy(start_checkpoint.lock);

    return 0;
}