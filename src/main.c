// main.c source code
#include "../headers/main.h"

int main(void)
{
    /**
     * in this varient, we will have two players, fst and snd
     * each one of them, will have its instance on the maze, which are just two references to the same maze on the disk
     * each one of them, will also have its corresponding entry/end, therefore each one will populate its own checkpoints track records.
     *
     * Note: we choosed to make the players also own thier set of threads and mutex locks, this is for more flexibility and less exposure
     * for errors, like seg faults or deadlocks.
     */

    /* instanciate the mazes */
    maze_t fst_maze;
    maze_t snd_maze;

    /* load the mazes */
    loadMaze(&fst_maze);
    loadMaze(&snd_maze);

    /* print one of the two maze's grids, since they are actually the same on the disk. */
    printMatrix(&(fst_maze.grid), fst_maze.row_count, fst_maze.col_count);

    /* load to the maze instances the entry/end coordinates*/
    findKeyPoint(&fst_maze, 3, 2);
    findKeyPoint(&snd_maze, 5, 2);

    /* instanciate two threads which will be our two players */
    pthread_t fst, snd;

    /* print the entry/end for the two players */
    printf("\nfst: Entry[%d][%d]\t-->\t End[%d][%d]\n", fst_maze.entry[0], fst_maze.entry[1], fst_maze.end[0], fst_maze.end[1]);
    printf("\nsnd: Entry[%d][%d]\t-->\t End[%d][%d]\n", snd_maze.entry[0], snd_maze.entry[1], snd_maze.end[0], snd_maze.end[1]);

    /* populate the fst player start checkpoint */
    checkpoint_t fst_checkpoint;
    fst_checkpoint.current_track_record = create_node_list(fst_maze.entry[0] * fst_maze.col_count + fst_maze.entry[1]);
    fst_checkpoint.direction = -(fst_maze.col_count);
    fst_checkpoint.end_reached = false;
    fst_checkpoint.p_maze = &fst_maze;
    fst_checkpoint.last_pos = fst_maze.entry[0] * fst_maze.col_count + fst_maze.entry[1];

    fst_checkpoint.limited_threads = true;
    fst_checkpoint.lock = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(fst_checkpoint.lock, NULL);

    int fst_limit = 6;
    fst_checkpoint.p_free_threads_count = &fst_limit;

    /* populate the snd player start checkpoint */
    checkpoint_t snd_checkpoint;
    snd_checkpoint.current_track_record = create_node_list(snd_maze.entry[0] * snd_maze.col_count + snd_maze.entry[1]);
    snd_checkpoint.direction = -(snd_maze.col_count);
    snd_checkpoint.end_reached = false;
    snd_checkpoint.p_maze = &snd_maze;
    snd_checkpoint.last_pos = snd_maze.entry[0] * snd_maze.col_count + snd_maze.entry[1];

    snd_checkpoint.limited_threads = true;
    snd_checkpoint.lock = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(snd_checkpoint.lock, NULL);

    int snd_limit = 6;
    snd_checkpoint.p_free_threads_count = &snd_limit;

    /* fire up the players in two threads */
    pthread_create(&fst, NULL, &solveMaze_threaded, &fst_checkpoint);
    pthread_create(&snd, NULL, &solveMaze_threaded, &snd_checkpoint);

    /* await for them until they reach the meetup point at the same time */
    pthread_join(fst, NULL);
    pthread_join(snd, NULL);

    if (getLength(fst_checkpoint.current_track_record) >= 1)
    {
        printf("\nPrinting the fst track record to the meetup point: \n");
        print_list(fst_checkpoint.current_track_record);
        fst_checkpoint.current_track_record = destroy_list(fst_checkpoint.current_track_record);
        printf("\n");
    }

    if (getLength(snd_checkpoint.current_track_record) >= 1)
    {
        printf("\nPrinting the snd track record to the meetup point: \n");
        print_list(snd_checkpoint.current_track_record);
        snd_checkpoint.current_track_record = destroy_list(snd_checkpoint.current_track_record);
        printf("\n");
    }

    /* destroy the mutexes since they can be held locked */
    pthread_mutex_destroy(fst_checkpoint.lock);
    pthread_mutex_destroy(snd_checkpoint.lock);

    /* load to the maze the fst and snd entry/end coordinates (this is the actual end, not the meetup point)*/
    findKeyPoint(&fst_maze, 2, 4);
    findKeyPoint(&snd_maze, 2, 6);

    /* print the entry/end for the two players */
    printf("\nfst: Entry[%d][%d]\t-->\t End[%d][%d]\n", fst_maze.entry[0], fst_maze.entry[1], fst_maze.end[0], fst_maze.end[1]);
    printf("\nsnd: Entry[%d][%d]\t-->\t End[%d][%d]\n", snd_maze.entry[0], snd_maze.entry[1], snd_maze.end[0], snd_maze.end[1]);

    /* repopulate the fst start checkpoint */
    fst_checkpoint.current_track_record = create_node_list(fst_maze.entry[0] * fst_maze.col_count + fst_maze.entry[1]);
    fst_checkpoint.direction = -(fst_maze.col_count);
    fst_checkpoint.end_reached = false;
    fst_checkpoint.p_maze = &fst_maze;
    fst_checkpoint.last_pos = fst_maze.entry[0] * fst_maze.col_count + fst_maze.entry[1];

    fst_checkpoint.limited_threads = true;
    fst_limit = 6;
    fst_checkpoint.p_free_threads_count = &fst_limit;

    fst_checkpoint.lock = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(fst_checkpoint.lock, NULL);

    /* repopulate the snd start checkpoint */
    snd_checkpoint.current_track_record = create_node_list(snd_maze.entry[0] * snd_maze.col_count + snd_maze.entry[1]);
    snd_checkpoint.direction = -(snd_maze.col_count);
    snd_checkpoint.end_reached = false;
    snd_checkpoint.p_maze = &snd_maze;
    snd_checkpoint.last_pos = snd_maze.entry[0] * snd_maze.col_count + snd_maze.entry[1];

    snd_checkpoint.limited_threads = true;
    snd_limit = 6;
    snd_checkpoint.p_free_threads_count = &snd_limit;
    

    snd_checkpoint.lock = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(snd_checkpoint.lock, NULL);

    /* fire up the threads for the two players, so as they can seek thier actual end*/
    pthread_create(&fst, NULL, &solveMaze_threaded, &fst_checkpoint);
    pthread_create(&snd, NULL, &solveMaze_threaded, &snd_checkpoint);

    /* await until they both arrive*/
    pthread_join(fst, NULL);
    pthread_join(snd, NULL);

    if (getLength(fst_checkpoint.current_track_record) >= 1)
    {
        printf("\nPrinting the fst track record to the end : \n");
        print_list(fst_checkpoint.current_track_record);
        fst_checkpoint.current_track_record = destroy_list(fst_checkpoint.current_track_record);
        printf("\n");
    }
    if (getLength(snd_checkpoint.current_track_record) >= 1)
    {
        printf("\nPrinting the snd track record to the end : \n");
        print_list(snd_checkpoint.current_track_record);
        snd_checkpoint.current_track_record = destroy_list(snd_checkpoint.current_track_record);
        printf("\n");
    }

    /* destroy the mazes to avoid memory leaks */
    destroyMaze(&fst_maze);
    destroyMaze(&snd_maze);

    /* destroy the locks to avoid memory leaks */
    pthread_mutex_destroy(fst_checkpoint.lock);
    pthread_mutex_destroy(snd_checkpoint.lock);

    return 0;
}