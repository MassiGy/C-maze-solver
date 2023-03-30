// main.c source code
#include "../headers/main.h"

int main(void)
{
    /**
        This varient of the program solves the maze with limied threads mode using a semaphore to handel the threads count.

        We basicaly init our semaphore to the number of threads that we set as our limit, then each time we fire up a threads
        we wait on the semaphore to decrement its value atomicaly. In the other hand, each time we join a thread, we signal/post
        our semaphore to increment it atomicaly.

        NOTE: just make sure that you give a high enough value for the semaphore init function, since otherwise can cause a deadlock
        because the thread that is called can be waiting to spawn another thread but it needs to pop out so as a new thread can be fired up.

        So if all threads fall in this situation, it is a deadlock.
    */

    /* instaciate our maze, and then load it from the maze.txt file.*/
    maze_t maze;
    loadMaze(&maze);
    /*print the grid*/
    printMatrix(&(maze.grid), maze.row_count, maze.col_count);
    /*populate the end and the start coordinates.*/
    findStart(&maze);
    findEnd(&maze);

    printf("\nEntry[%d][%d]\t-->\t End[%d][%d]\n", maze.entry[0], maze.entry[1], maze.end[0], maze.end[1]);

    /*populate our start checkpoint*/
    checkpoint_t start_checkpoint;
    start_checkpoint.current_track_record = create_node_list(maze.entry[0] * maze.col_count + maze.entry[1]);
    start_checkpoint.direction = (-maze.col_count);
    start_checkpoint.end_reached = false;
    start_checkpoint.p_maze = &maze;
    start_checkpoint.last_pos = maze.entry[0] * maze.col_count + maze.entry[1];

    start_checkpoint.limited_threads = true;
    start_checkpoint.free_threads_count = malloc(sizeof(sem_t));
    /* init our semaphore */
    sem_init(start_checkpoint.free_threads_count, 0, 10);

    solveMaze_threaded(&start_checkpoint);

    if (getLength(start_checkpoint.current_track_record) >= 1)
    {
        print_list(start_checkpoint.current_track_record);
        start_checkpoint.current_track_record = destroy_list(start_checkpoint.current_track_record);
    }

    /* free everthing to prevent memory leaks */
    destroyMaze(&maze);
    sem_destroy(start_checkpoint.free_threads_count);
    /*make sure to free the semaphore to prevent any memory leak*/
    free(start_checkpoint.free_threads_count);

    return 0;
}