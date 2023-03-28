// main.c source code
#include "../headers/main.h"

/*
rec: ./main  40.17s user 0.07s system 197% cpu 20.344 total

*/

int main(void)
{

    maze_t fst_maze;
    maze_t snd_maze;
    loadMaze(&fst_maze);
    loadMaze(&snd_maze);
    printMatrix(&(fst_maze.grid), fst_maze.row_count, fst_maze.col_count);
    findKeyPoint(&fst_maze, 3, 2);
    findKeyPoint(&snd_maze, 5, 2);

    pthread_t fst, snd;

    printf("Entrée : ");
    for (int i = 0; i < 2; i++)
    {
        printf("[%d]", fst_maze.entry[i]);
    }

    printf("\nSortie : ");
    for (int i = 0; i < 2; i++)
    {
        printf("[%d]", fst_maze.end[i]);
    }

    printf("\n");
    printf("Entrée : ");
    for (int i = 0; i < 2; i++)
    {
        printf("[%d]", snd_maze.entry[i]);
    }

    printf("\nSortie : ");
    for (int i = 0; i < 2; i++)
    {
        printf("[%d]", snd_maze.end[i]);
    }

    printf("\n");

    checkpoint_t fst_checkpoint;
    fst_checkpoint.current_track_record = create_node_list(fst_maze.entry[0] * fst_maze.col_count + fst_maze.entry[1]);
    fst_checkpoint.direction = -(fst_maze.col_count);
    fst_checkpoint.end_reached = false;
    fst_checkpoint.p_maze = &fst_maze;
    fst_checkpoint.last_pos = fst_maze.entry[0] * fst_maze.col_count + fst_maze.entry[1];

    fst_checkpoint.limited_threads = false;
    fst_checkpoint.lock = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(fst_checkpoint.lock, NULL);

    int fst_limit = 20;
    fst_checkpoint.p_free_threads_count = &fst_limit;

    checkpoint_t snd_checkpoint;
    snd_checkpoint.current_track_record = create_node_list(snd_maze.entry[0] * snd_maze.col_count + snd_maze.entry[1]);
    snd_checkpoint.direction = -(snd_maze.col_count);
    snd_checkpoint.end_reached = false;
    snd_checkpoint.p_maze = &snd_maze;
    snd_checkpoint.last_pos = snd_maze.entry[0] * snd_maze.col_count + snd_maze.entry[1];

    snd_checkpoint.limited_threads = false;
    snd_checkpoint.lock = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(snd_checkpoint.lock, NULL);

    int snd_limit = 20;
    snd_checkpoint.p_free_threads_count = &snd_limit;

    pthread_create(&fst, NULL, &solveMaze_threaded, &fst_checkpoint);
    pthread_create(&snd, NULL, &solveMaze_threaded, &snd_checkpoint);

    pthread_join(fst, NULL);
    pthread_join(snd, NULL);
    printf("~~~~~~~~~~~~~\n");

    if (getLength(fst_checkpoint.current_track_record) >= 1)
    {
        print_list(fst_checkpoint.current_track_record);
        fst_checkpoint.current_track_record = destroy_list(fst_checkpoint.current_track_record);
    }
    printf("###########\n");
    if (getLength(snd_checkpoint.current_track_record) >= 1)
    {
        print_list(snd_checkpoint.current_track_record);
        snd_checkpoint.current_track_record = destroy_list(snd_checkpoint.current_track_record);
    }
    pthread_mutex_destroy(fst_checkpoint.lock);
    pthread_mutex_destroy(snd_checkpoint.lock);

    findKeyPoint(&fst_maze, 2, 4);
    findKeyPoint(&snd_maze, 2, 6);
    fst_checkpoint.current_track_record = create_node_list(fst_maze.entry[0] * fst_maze.col_count + fst_maze.entry[1]);
    fst_checkpoint.direction = -(fst_maze.col_count);
    fst_checkpoint.end_reached = false;
    fst_checkpoint.p_maze = &fst_maze;
    fst_checkpoint.last_pos = fst_maze.entry[0] * fst_maze.col_count + fst_maze.entry[1];

    fst_checkpoint.limited_threads = false;

    fst_checkpoint.lock = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(fst_checkpoint.lock, NULL);

    snd_checkpoint.current_track_record = create_node_list(snd_maze.entry[0] * snd_maze.col_count + snd_maze.entry[1]);
    snd_checkpoint.direction = -(snd_maze.col_count);
    snd_checkpoint.end_reached = false;
    snd_checkpoint.p_maze = &snd_maze;
    snd_checkpoint.last_pos = snd_maze.entry[0] * snd_maze.col_count + snd_maze.entry[1];

    snd_checkpoint.limited_threads = false;

    snd_checkpoint.lock = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(snd_checkpoint.lock, NULL);

    pthread_create(&fst, NULL, &solveMaze_threaded, &fst_checkpoint);
    pthread_create(&snd, NULL, &solveMaze_threaded, &snd_checkpoint);

    pthread_join(fst, NULL);
    pthread_join(snd, NULL);

    if (getLength(fst_checkpoint.current_track_record) >= 1)
    {
        print_list(fst_checkpoint.current_track_record);
        fst_checkpoint.current_track_record = destroy_list(fst_checkpoint.current_track_record);
    }
    printf("###########\n");
    if (getLength(snd_checkpoint.current_track_record) >= 1)
    {
        print_list(snd_checkpoint.current_track_record);
        snd_checkpoint.current_track_record = destroy_list(snd_checkpoint.current_track_record);
    }

    destroyMaze(&fst_maze);
    destroyMaze(&snd_maze);
    pthread_mutex_destroy(fst_checkpoint.lock);
    pthread_mutex_destroy(snd_checkpoint.lock);

    return 0;
}