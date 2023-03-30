// main.c source code
#include "../headers/main.h"
/**
 * when thereis no meetup, everything is ok
 *
 * on meetup mode,
 * When the threads are limited, and its on recursive, everything is ok.
 * When the threads are not limited, it goes through the meet point and then to the end, but it goes way beyond the end.
 * When the threads are limited, and its not only recursive, it goes through the meet point and then to the end, but it goes way beyond the end. (unless you give it
 * a small limit count, which will cause a seg fault.)
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

    /* print the entry/end for the two players */
    printf("\nfst: Entry[%d][%d]\t-->\t End[%d][%d]\n", fst_maze.entry[0], fst_maze.entry[1], fst_maze.end[0], fst_maze.end[1]);
    // printf("\nsnd: Entry[%d][%d]\t-->\t End[%d][%d]\n", snd_maze.entry[0], snd_maze.entry[1], snd_maze.end[0], snd_maze.end[1]);

    int thread_limit_count = 10;

    checkpoint_t fst_checkpoint;
    fst_checkpoint.current_track_record = create_node_list(fst_maze.entry[0] * fst_maze.col_count + fst_maze.entry[1]);

    fst_checkpoint.direction = -(fst_maze.col_count);
    fst_checkpoint.end_reached = false;
    fst_checkpoint.p_maze = &fst_maze;
    fst_checkpoint.last_pos = fst_maze.entry[0] * fst_maze.col_count + fst_maze.entry[1];
    fst_checkpoint.id = 0;
    fst_checkpoint.thereis_meet_up = true;
    fst_checkpoint.meet_point_reached = false;
    bool fst_thereis_meetup = true; // this is for the printing

    fst_checkpoint.limited_threads = false;
    fst_checkpoint.lock = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(fst_checkpoint.lock, NULL);

    fst_checkpoint.p_free_threads_count = &thread_limit_count;

    /* second player */
    // checkpoint_t snd_checkpoint;
    // snd_checkpoint.current_track_record = create_node_list(snd_maze.entry[0] * snd_maze.col_count + snd_maze.entry[1]);

    // snd_checkpoint.direction = -(snd_maze.col_count);
    // snd_checkpoint.end_reached = false;
    // snd_checkpoint.p_maze = &snd_maze;
    // snd_checkpoint.last_pos = snd_maze.entry[0] * snd_maze.col_count + snd_maze.entry[1];
    // snd_checkpoint.id = 1;
    // snd_checkpoint.thereis_meet_up = true;
    // snd_checkpoint.meet_point_reached = false;
    // bool snd_thereis_meetup = true;

    // snd_checkpoint.limited_threads = true;
    // snd_checkpoint.lock = fst_checkpoint.lock;

    // snd_checkpoint.p_free_threads_count = &thread_limit_count;

    pthread_t fst, snd;

    pthread_create(&fst, NULL, &solveMaze_threaded, &fst_checkpoint);
    // pthread_create(&snd, NULL, &solveMaze_threaded, &snd_checkpoint);

    pthread_join(fst, NULL);
    // pthread_join(snd, NULL);

    printf("~~~~~~~~~~~~~~\n");

    if (fst_thereis_meetup && getLength(fst_checkpoint.path_to_meet_point) >= 1)
    {
        print_list(fst_checkpoint.path_to_meet_point);
        fst_checkpoint.path_to_meet_point = destroy_list(fst_checkpoint.path_to_meet_point);
    }

    printf("############\n");
    if (getLength(fst_checkpoint.current_track_record) >= 1)
    {
        print_list(fst_checkpoint.current_track_record);
        fst_checkpoint.current_track_record = destroy_list(fst_checkpoint.current_track_record);
    }

    // printf("@@@@@@@@@@@@@@@@@@@@@@@ Player 2 @@@@@@@@@\n");

    //     if (getLength(snd_checkpoint.path_to_meet_point) >= 1)
    //     {
    //         print_list(snd_checkpoint.path_to_meet_point);
    //         snd_checkpoint.path_to_meet_point = destroy_list(snd_checkpoint.path_to_meet_point);
    //     }

    // if (getLength(snd_checkpoint.current_track_record) >= 1)
    // {
    //     print_list(snd_checkpoint.current_track_record);
    //     snd_checkpoint.current_track_record = destroy_list(snd_checkpoint.current_track_record);
    // }

    destroyMaze(&fst_maze);
    // destroyMaze(&snd_maze);
    pthread_mutex_destroy(fst_checkpoint.lock);

    return 0;
}