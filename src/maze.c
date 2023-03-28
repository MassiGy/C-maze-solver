#include "../headers/maze.h"

void findStart(maze_t *playground)
{
    for (int i = 0; i < playground->row_count; i++)
    {
        for (int j = 0; j < playground->col_count; j++)
        {
            if (playground->grid[i][j] == 2)
            {
                playground->entry[0] = i;
                playground->entry[1] = j;
                return;
            }
        }
    }
}

void findEnd(maze_t *playground)
{
    for (int i = 0; i < playground->row_count; i++)
    {
        for (int j = 0; j < playground->col_count; j++)
        {
            if (playground->grid[i][j] == 3)
            {
                playground->end[0] = i;
                playground->end[1] = j;
                return;
            }
        }
    }
}

void loadMaze(maze_t *playground)
{
    assert(playground != NULL);

    /* use wc command to get the lines count*/
    int res = system("wc --lines ../dist/maze.txt > ../dist/maze_lines_count.txt");
    assert(res == 0);

    /* use wc command to get the cols count*/
    res = system("head -n 1 ../dist/maze.txt | wc -c > ../dist/maze_cols_count.txt");
    assert(res == 0);

    /* read the lines count and stock it to the structure */
    FILE *_file = fopen("../dist/maze_lines_count.txt", "r");
    assert(_file != NULL);

    fscanf(_file, "%d", &(playground->row_count));
    fclose(_file);

    /* read the cols count and stock it to the structure */
    _file = fopen("../dist/maze_cols_count.txt", "r");
    assert(_file != NULL);

    fscanf(_file, "%d", &(playground->col_count));
    // adjust the col_count by removing the last character since it is a \n
    playground->col_count--;

    fclose(_file);

    /* create the matrix */
    playground->grid = createMatrix(playground->row_count, playground->col_count);

    /* read the file to populate the grid */
    _file = fopen("../dist/maze.txt", "r");

    /* init a buffer to read the lines */
    char buffer[playground->col_count];
    /* init a byte to read char by char from the buffer, since atoi take a string, and our string
    can only be of length 1
    */
    char byte[1];

    for (int i = 0; i < playground->row_count; i++)
    {
        /* read a line*/
        fscanf(_file, "%s", buffer);

        for (int j = 0; j < playground->col_count; j++)
        {
            byte[0] = buffer[j];
            /* read the int and load them to the matrix */
            playground->grid[i][j] = atoi(&(byte[0]));
        }
    }

    fclose(_file);
    return;
}

int get_line(int el_pos, int maze_col_count)
{
    return (el_pos / maze_col_count);
}
int get_colomn(int el_pos, int maze_col_count)
{
    return (el_pos % maze_col_count);
}
bool can_go_up(int current_line, int current_col, maze_t *maze, list_t *track_record)
{
    int son_next_move = ((current_line - 1) * maze->col_count + current_col);

    bool result = (current_line - 1) >= 0;
    result = result && (maze->grid[current_line - 1][current_col]) != 0;

    int track_record_length = getLength(track_record);
    int *track_record_vect = listToArray(track_record, track_record_length);

    result = result && !(liniar_search_array(track_record_vect, track_record_length, son_next_move));

    free(track_record_vect);

    return result;
}
bool can_go_down(int current_line, int current_col, maze_t *maze, list_t *track_record)
{
    int son_next_move = (current_line + 1) * maze->col_count + current_col;

    bool result = (current_line + 1 < maze->row_count);
    result = result && maze->grid[current_line + 1][current_col] != 0;

    int track_record_length = getLength(track_record);
    int *track_record_vect = listToArray(track_record, track_record_length);

    result = result && !(liniar_search_array(track_record_vect, track_record_length, son_next_move));

    free(track_record_vect);

    return result;
}
bool can_go_right(int current_line, int current_col, maze_t *maze, list_t *track_record)
{
    int son_next_move = current_line * maze->col_count + current_col + 1;

    bool result = (current_col + 1 < maze->col_count);
    result = result && (maze->grid[current_line][current_col + 1] != 0);

    int track_record_length = getLength(track_record);
    int *track_record_vect = listToArray(track_record, track_record_length);

    result = result && !(liniar_search_array(track_record_vect, track_record_length, son_next_move));

    free(track_record_vect);

    return result;
}
bool can_go_left(int current_line, int current_col, maze_t *maze, list_t *track_record)
{

    int son_next_move = current_line * maze->col_count + current_col - 1;

    bool result = (current_col - 1) >= 0;
    result = result && maze->grid[current_line][current_col - 1] != 0;

    int track_record_length = getLength(track_record);
    int *track_record_vect = listToArray(track_record, track_record_length);

    result = result && !(liniar_search_array(track_record_vect, track_record_length, son_next_move));

    free(track_record_vect);

    return result;
}

void *solveMaze_threaded(void *checkpoint)
{

    assert(checkpoint != NULL);

    // get the structs
    checkpoint_t *current_checkpoint = (checkpoint_t *)checkpoint;
    maze_t maze = *(current_checkpoint->p_maze);

    // get the coordinates
    int current_line = get_line(current_checkpoint->last_pos, maze.col_count);
    int current_col = get_colomn(current_checkpoint->last_pos, maze.col_count);

    // make sure that the parent is on its track record
    assert(current_checkpoint->current_track_record != NULL);

    int next_move;
    bool is_up_possible = false;
    bool is_down_possible = false;
    bool is_left_possible = false;
    bool is_right_possible = false;
    pthread_t up_thread, down_thread, left_thread, right_thread;
    checkpoint_t up_thread_checkpoint, down_thread_checkpoint, left_thread_checkpoint, right_thread_checkpoint;

    bool is_up_thread_launched;
    bool is_down_thread_launched;
    bool is_left_thread_launched;
    bool is_right_thread_launched;

    if (current_checkpoint->limited_threads)
    {
        /* if the threads are limited, suppose that at the beginning no threads are launched */
        /* these will be our flags, and they will determine which thread is run */
        is_up_thread_launched = false;
        is_down_thread_launched = false;
        is_left_thread_launched = false;
        is_right_thread_launched = false;
    }

    int *constructed_path;
    int constructed_path_length;
    bool does_lead_to_end;

    while (!(current_checkpoint->end_reached))
    {
        /* reload the currect coordinates each time */
        current_line = get_line(current_checkpoint->last_pos, maze.col_count);
        current_col = get_colomn(current_checkpoint->last_pos, maze.col_count);

        // see if we are not on a boundry
        if (current_line >= maze.row_count || current_line < 0)
            break;
        if (current_col >= maze.col_count || current_col < 0)
            break;

        /* make sure that we are not blocked */
        if (maze.grid[current_line][current_col] == 0)
            break;

        /* calc the next move to see if any of the possible ways is also the parent way */
        next_move = current_checkpoint->last_pos + current_checkpoint->direction;

        /*check for any other possible ways*/
        if (current_checkpoint->direction != -maze.col_count)
        {
            is_up_possible = can_go_up(current_line, current_col, &maze, current_checkpoint->current_track_record);
        }
        if (current_checkpoint->direction != maze.col_count)
        {
            is_down_possible = can_go_down(current_line, current_col, &maze, current_checkpoint->current_track_record);
        }
        if (current_checkpoint->direction != -1)
        {
            is_left_possible = can_go_left(current_line, current_col, &maze, current_checkpoint->current_track_record);
        }
        if (current_checkpoint->direction != 1)
        {
            is_right_possible = can_go_right(current_line, current_col, &maze, current_checkpoint->current_track_record);
        }

        /* if there is any way, fire up a thread following it */
        if (is_up_possible)
        {
            // create a thread to go up

            /* make sure that the parent<>son relation is set */
            up_thread_checkpoint.current_track_record = copy_list(current_checkpoint->current_track_record);

            /* make sure that the passed direction is the correct one */
            up_thread_checkpoint.direction = -(maze.col_count);
            /* make sure that the last_position is passed as the son next move according to its direction*/
            up_thread_checkpoint.last_pos = current_checkpoint->last_pos - (maze.col_count);
            /* make sure that the son also goes to search for the end */
            up_thread_checkpoint.end_reached = current_checkpoint->end_reached;
            /* make sure that the son also gets a refrence to the maze */
            up_thread_checkpoint.p_maze = &maze;

            /* make sure that the son knows if the threads are limited */
            up_thread_checkpoint.limited_threads = current_checkpoint->limited_threads;
            /* if so make sure that the son has the refrence to the mutex and the counter */
            up_thread_checkpoint.p_free_threads_count = current_checkpoint->p_free_threads_count;
            up_thread_checkpoint.lock = current_checkpoint->lock;

            if (current_checkpoint->limited_threads)
            {
                /*make sure that there is a free thread before spawning one*/
                bool thereis_free_thread = false;

                /* lock the critical region */
                pthread_mutex_lock(current_checkpoint->lock);

                if ((*(current_checkpoint->p_free_threads_count)) > 0)
                {
                    thereis_free_thread = true;
                    (*(current_checkpoint->p_free_threads_count))--;
                    printf("on up check (%p) => (%d)\n", current_checkpoint->p_free_threads_count, (*(current_checkpoint->p_free_threads_count)));
                }
                pthread_mutex_unlock(current_checkpoint->lock);

                if (thereis_free_thread)
                {
                    /* if there is a free thread, fire up the thread and make the is_thread_launched flag on.*/
                    is_up_thread_launched = true;
                    /* push the next son move to its track record */
                    up_thread_checkpoint.current_track_record = push_list(up_thread_checkpoint.current_track_record, current_checkpoint->last_pos - (maze.col_count));
                    pthread_create(&up_thread, NULL, &solveMaze_threaded, &up_thread_checkpoint);
                }
                else
                {
                    /* otherwise, solve this maze part recursively, no need to push the next move of the thread to its record */
                    is_up_thread_launched = false;
                    /* for the recursive, no need to push the son thread to next pos, since it manages it by itself*/
                    up_thread_checkpoint.last_pos = current_checkpoint->last_pos;
                    solveMaze_rec(&maze,
                                  &(up_thread_checkpoint.current_track_record),
                                  get_line(up_thread_checkpoint.last_pos, maze.col_count),
                                  get_colomn(up_thread_checkpoint.last_pos, maze.col_count));
                }
            }
            else
            {
                /* push the next son move to its track record */
                up_thread_checkpoint.current_track_record = push_list(up_thread_checkpoint.current_track_record, current_checkpoint->last_pos - (maze.col_count));
                pthread_create(&up_thread, NULL, &solveMaze_threaded, &up_thread_checkpoint);
            }
        }

        if (is_down_possible)
        {
            // create a thread to go down

            /* make sure that the parent<>son relation is set */
            down_thread_checkpoint.current_track_record = copy_list(current_checkpoint->current_track_record);

            /* make sure that the passed direction is the correct one */
            down_thread_checkpoint.direction = +(maze.col_count);
            /* make sure that the last_position is passed as the son next move according to its direction*/
            down_thread_checkpoint.last_pos = current_checkpoint->last_pos + (maze.col_count);
            /* make sure that the son also goes to search for the end */
            down_thread_checkpoint.end_reached = current_checkpoint->end_reached;
            /* make sure that the son also gets a refrence to the maze */
            down_thread_checkpoint.p_maze = &maze;

            /* make sure that the son knows if the threads are limited */
            down_thread_checkpoint.limited_threads = current_checkpoint->limited_threads;
            /* make sure that the son has the refrence to the mutex and the counter */
            down_thread_checkpoint.p_free_threads_count = current_checkpoint->p_free_threads_count;
            down_thread_checkpoint.lock = current_checkpoint->lock;

            if (current_checkpoint->limited_threads)
            {
                /*make sure that there is a free thread before spawning one*/
                bool thereis_free_thread = false;

                /*lock the shared ressources*/
                pthread_mutex_lock(current_checkpoint->lock);

                if ((*(current_checkpoint->p_free_threads_count)) > 0)
                {
                    thereis_free_thread = true;
                    (*(current_checkpoint->p_free_threads_count))--;
                    printf("on down check (%p) => (%d)\n", current_checkpoint->p_free_threads_count, (*(current_checkpoint->p_free_threads_count)));
                }

                pthread_mutex_unlock(current_checkpoint->lock);

                if (thereis_free_thread)
                {
                    /* if there is any thread availible, launche it and make the flag is_thread_launched on*/
                    is_down_thread_launched = true;
                    /* push the next son move to its track record */
                    down_thread_checkpoint.current_track_record = push_list(down_thread_checkpoint.current_track_record, current_checkpoint->last_pos + maze.col_count);
                    pthread_create(&down_thread, NULL, &solveMaze_threaded, &down_thread_checkpoint);
                }
                else
                {
                    /* otherwise, solve this maze part recursively */
                    is_down_thread_launched = false;
                    /* for the recursive, no need to push the son thread to next pos, since it manages it by itself*/
                    down_thread_checkpoint.last_pos = current_checkpoint->last_pos;
                    solveMaze_rec(&maze,
                                  &(down_thread_checkpoint.current_track_record),
                                  get_line(down_thread_checkpoint.last_pos, maze.col_count),
                                  get_colomn(down_thread_checkpoint.last_pos, maze.col_count));
                }
            }
            else
            {
                /* push the next son move to its track record */
                down_thread_checkpoint.current_track_record = push_list(down_thread_checkpoint.current_track_record, current_checkpoint->last_pos + maze.col_count);
                pthread_create(&down_thread, NULL, &solveMaze_threaded, &down_thread_checkpoint);
            }
        }
        if (is_left_possible)
        {
            // create a thread to go left

            /* make sure that the parent<>son relation is set */
            left_thread_checkpoint.current_track_record = copy_list(current_checkpoint->current_track_record);

            /* make sure that the passed direction is the correct one */
            left_thread_checkpoint.direction = -(1);
            /* make sure that the last_position is passed as the son next move according to its direction*/
            left_thread_checkpoint.last_pos = current_checkpoint->last_pos - 1;
            /* make sure that the son also goes to search for the end */
            left_thread_checkpoint.end_reached = current_checkpoint->end_reached;
            /* make sure that the son also gets a refrence to the maze */
            left_thread_checkpoint.p_maze = &maze;

            /* make sure that the son knows if the threads are limited */
            left_thread_checkpoint.limited_threads = current_checkpoint->limited_threads;
            /* make sure that the son has the refrence to the mutex and the counter */
            left_thread_checkpoint.p_free_threads_count = current_checkpoint->p_free_threads_count;
            left_thread_checkpoint.lock = current_checkpoint->lock;

            if (current_checkpoint->limited_threads)
            {
                /*make sure that there is a free thread before spawning one*/
                bool thereis_free_thread = false;

                /* lock the critical region */
                pthread_mutex_lock(current_checkpoint->lock);

                if ((*(current_checkpoint->p_free_threads_count)) > 0)
                {
                    thereis_free_thread = true;
                    (*(current_checkpoint->p_free_threads_count))--;
                    printf("on left check (%p) => (%d)\n", current_checkpoint->p_free_threads_count, (*(current_checkpoint->p_free_threads_count)));
                }

                pthread_mutex_unlock(current_checkpoint->lock);

                if (thereis_free_thread)
                {
                    /* if there is any availible thread, launch it, and make the is_thread_launched flag on */
                    is_left_thread_launched = true;
                    /* push the next son move to its track record */
                    left_thread_checkpoint.current_track_record = push_list(left_thread_checkpoint.current_track_record, current_checkpoint->last_pos - 1);
                    pthread_create(&left_thread, NULL, &solveMaze_threaded, &left_thread_checkpoint);
                }
                else
                {
                    /* otherwise solve this maze part recursively */
                    is_left_thread_launched = false;
                    /* for the recursive, no need to push the son thread to next pos, since it manages it by itself*/
                    left_thread_checkpoint.last_pos = current_checkpoint->last_pos;
                    solveMaze_rec(&maze,
                                  &(left_thread_checkpoint.current_track_record),
                                  get_line(left_thread_checkpoint.last_pos, maze.col_count),
                                  get_colomn(left_thread_checkpoint.last_pos, maze.col_count));
                }
            }
            else
            {
                /* push the next son move to its track record */
                left_thread_checkpoint.current_track_record = push_list(left_thread_checkpoint.current_track_record, current_checkpoint->last_pos - 1);
                pthread_create(&left_thread, NULL, &solveMaze_threaded, &left_thread_checkpoint);
            }
        }

        if (is_right_possible)
        {
            // create a thread to go right

            /* make sure that the parent<>son relation is set */
            right_thread_checkpoint.current_track_record = copy_list(current_checkpoint->current_track_record);

            /* make sure that the passed direction is the correct one */
            right_thread_checkpoint.direction = (1);
            /* make sure that the last_position is passed as the son next move according to its direction*/
            right_thread_checkpoint.last_pos = current_checkpoint->last_pos + 1;
            /* make sure that the son also goes to search for the end */
            right_thread_checkpoint.end_reached = current_checkpoint->end_reached;
            /* make sure that the son also gets a refrence to the maze */
            right_thread_checkpoint.p_maze = &maze;

            /* make sure that the son has the refrence to the mutex and the counter */
            right_thread_checkpoint.limited_threads = current_checkpoint->limited_threads;
            right_thread_checkpoint.p_free_threads_count = current_checkpoint->p_free_threads_count;
            right_thread_checkpoint.lock = current_checkpoint->lock;

            if (current_checkpoint->limited_threads)
            {
                /*make sure that there is a free thread before spawning one*/
                bool thereis_free_thread = false;

                /*lock the critical region*/
                pthread_mutex_lock(current_checkpoint->lock);

                if ((*(current_checkpoint->p_free_threads_count)) > 0)
                {
                    thereis_free_thread = true;
                    (*(current_checkpoint->p_free_threads_count))--;
                    printf("on right check (%p) => (%d)\n", current_checkpoint->p_free_threads_count, (*(current_checkpoint->p_free_threads_count)));
                }

                pthread_mutex_unlock(current_checkpoint->lock);

                if (thereis_free_thread)
                {
                    /* if there is any thread availible from our pool, launch it and turn on the is_thread_lanched flag*/
                    is_right_thread_launched = true;
                    /* push the next son move to its track record */
                    right_thread_checkpoint.current_track_record = push_list(right_thread_checkpoint.current_track_record, current_checkpoint->last_pos + 1);
                    pthread_create(&right_thread, NULL, &solveMaze_threaded, &right_thread_checkpoint);
                }
                else
                {
                    /*otherwise, solve this part of the maze recursively*/
                    is_right_thread_launched = false;
                    /* for the recursive, no need to push the son thread to next pos, since it manages it by itself*/
                    right_thread_checkpoint.last_pos = current_checkpoint->last_pos;
                    solveMaze_rec(&maze,
                                  &(right_thread_checkpoint.current_track_record),
                                  get_line(right_thread_checkpoint.last_pos, maze.col_count),
                                  get_colomn(right_thread_checkpoint.last_pos, maze.col_count));
                }
            }
            else
            {
                /* push the next son move to its track record */
                right_thread_checkpoint.current_track_record = push_list(right_thread_checkpoint.current_track_record, current_checkpoint->last_pos + 1);
                pthread_create(&right_thread, NULL, &solveMaze_threaded, &right_thread_checkpoint);
            }
        }

        /* if any thread gets spawned, we will wait for it */
        if (is_up_possible)
        {

            if (current_checkpoint->limited_threads)
            {
                if (is_up_thread_launched)
                {
                    /*if the launched thread flag is on, join the thread*/
                    pthread_join(up_thread, NULL);
                    /*signal the free_threads_counter value */
                    pthread_mutex_lock(current_checkpoint->lock);
                    (*(current_checkpoint->p_free_threads_count))++;
                    printf("on up join (%p) => (%d)\n", current_checkpoint->p_free_threads_count, (*(current_checkpoint->p_free_threads_count)));
                    pthread_mutex_unlock(current_checkpoint->lock);
                }
            }
            else
            {
                /*if the threads are not limited, join the called thread*/
                pthread_join(up_thread, NULL);
            }

            /* see if the constructed path by this thread leads to the end */
            constructed_path_length = getLength(up_thread_checkpoint.current_track_record);
            constructed_path = listToArray(up_thread_checkpoint.current_track_record, constructed_path_length);

            does_lead_to_end = liniar_search_array(constructed_path, constructed_path_length, maze.end[0] * maze.col_count + maze.end[1]);

            if (does_lead_to_end)
            {

                /* replace the parent track record with the son track record */
                destroy_list(current_checkpoint->current_track_record);
                current_checkpoint->current_track_record = up_thread_checkpoint.current_track_record;

                /*make the direction null, to halt the parent*/
                current_checkpoint->direction = 0;
                current_checkpoint->end_reached = true;
                current_checkpoint->last_pos = maze.end[0] * maze.col_count + maze.end[1];

                /* free the array */
                free(constructed_path);
                /*ignore the following and break out from the loop */
                break;
            }
            else
            {
                /* just free everything related to the thread */
                destroy_list(up_thread_checkpoint.current_track_record);
                free(constructed_path);
            }
        }

        if (is_down_possible)
        {

            if (current_checkpoint->limited_threads)
            {
                if (is_down_thread_launched)
                {
                    /*if the launched thread flag is on, join the thread*/
                    pthread_join(down_thread, NULL);

                    /*signal the free_threads_counter value */
                    pthread_mutex_lock(current_checkpoint->lock);
                    (*(current_checkpoint->p_free_threads_count))++;
                    printf("on down join (%p) => (%d)\n", current_checkpoint->p_free_threads_count, (*(current_checkpoint->p_free_threads_count)));

                    pthread_mutex_unlock(current_checkpoint->lock);
                }
            }
            else
            {
                /*if the threads are not limited, join the called thread*/
                pthread_join(down_thread, NULL);
            }

            /* see if the constructed path by this thread leads to the end */
            constructed_path_length = getLength(down_thread_checkpoint.current_track_record);
            constructed_path = listToArray(down_thread_checkpoint.current_track_record, constructed_path_length);

            does_lead_to_end = liniar_search_array(constructed_path, constructed_path_length, maze.end[0] * maze.col_count + maze.end[1]);

            if (does_lead_to_end)
            {
                /* replace the parent track record with the son track record */
                destroy_list(current_checkpoint->current_track_record);
                current_checkpoint->current_track_record = down_thread_checkpoint.current_track_record;

                /*make the direction null, to halt the parent*/
                current_checkpoint->direction = 0;
                current_checkpoint->end_reached = true;
                current_checkpoint->last_pos = maze.end[0] * maze.col_count + maze.end[1];

                /* free the array */
                free(constructed_path);
                /*ignore the following and break out from the loop */
                break;
            }
            else
            {
                /* just free everything related to the thread */
                destroy_list(down_thread_checkpoint.current_track_record);
                free(constructed_path);
            }
        }
        if (is_left_possible)
        {
            if (current_checkpoint->limited_threads)
            {
                if (is_left_thread_launched)
                {
                    /*if the launched thread flag is on, join the thread*/
                    pthread_join(left_thread, NULL);
                    /*signal the free_threads_counter value */
                    pthread_mutex_lock(current_checkpoint->lock);
                    (*(current_checkpoint->p_free_threads_count))++;
                    printf("on left join (%p) => (%d)\n", current_checkpoint->p_free_threads_count, (*(current_checkpoint->p_free_threads_count)));

                    pthread_mutex_unlock(current_checkpoint->lock);
                }
            }
            else
            {
                /*if the threads are not limited, join the called thread*/
                pthread_join(left_thread, NULL);
            }

            /* see if the constructed path by this thread leads to the end */
            constructed_path_length = getLength(left_thread_checkpoint.current_track_record);
            constructed_path = listToArray(left_thread_checkpoint.current_track_record, constructed_path_length);

            does_lead_to_end = liniar_search_array(constructed_path, constructed_path_length, maze.end[0] * maze.col_count + maze.end[1]);

            if (does_lead_to_end)
            {
                /* replace the parent track record with the son track record */
                destroy_list(current_checkpoint->current_track_record);
                current_checkpoint->current_track_record = left_thread_checkpoint.current_track_record;

                /*make the direction null, to halt the parent*/
                current_checkpoint->direction = 0;
                current_checkpoint->end_reached = true;
                current_checkpoint->last_pos = maze.end[0] * maze.col_count + maze.end[1];

                /* free the array */
                free(constructed_path);
                /*ignore the following and break out from the loop */
                break;
            }
            else
            {

                /* just free everything related to the thread */
                destroy_list(left_thread_checkpoint.current_track_record);
                free(constructed_path);
            }
        }

        if (is_right_possible)
        {
            if (current_checkpoint->limited_threads)
            {
                if (is_right_thread_launched)
                {
                    /*if the launched thread flag is on, join the thread*/
                    pthread_join(right_thread, NULL);

                    /*signal the free_threads_counter value */
                    pthread_mutex_lock(current_checkpoint->lock);
                    (*(current_checkpoint->p_free_threads_count))++;
                    printf("on right join (%p) => (%d)\n", current_checkpoint->p_free_threads_count, (*(current_checkpoint->p_free_threads_count)));

                    pthread_mutex_unlock(current_checkpoint->lock);
                }
            }
            else
            {
                /*if the threads are not limited, join the called thread*/
                pthread_join(right_thread, NULL);
            }

            /* see if the constructed path by this thread leads to the end */
            constructed_path_length = getLength(right_thread_checkpoint.current_track_record);
            constructed_path = listToArray(right_thread_checkpoint.current_track_record, constructed_path_length);

            does_lead_to_end = liniar_search_array(constructed_path, constructed_path_length, maze.end[0] * maze.col_count + maze.end[1]);

            if (does_lead_to_end)
            {
                /* replace the parent track record with the son track record */
                destroy_list(current_checkpoint->current_track_record);
                current_checkpoint->current_track_record = right_thread_checkpoint.current_track_record;

                /*make the direction null, to halt the parent*/
                current_checkpoint->direction = 0;
                current_checkpoint->end_reached = true;
                current_checkpoint->last_pos = maze.end[0] * maze.col_count + maze.end[1];

                /* free the array */
                free(constructed_path);
                /*ignore the following and break out from the loop */
                break;
            }
            else
            {
                /* just free everything related to the thread */
                destroy_list(right_thread_checkpoint.current_track_record);
                free(constructed_path);
            }
        }

        /* then just continue moving according to the specified direction */
        current_checkpoint->last_pos = next_move;
        current_checkpoint->current_track_record = push_list(current_checkpoint->current_track_record, next_move);
        current_checkpoint->end_reached = (current_checkpoint->last_pos == maze.end[0] * maze.col_count + maze.end[1]);
    }

    return NULL;
}

void solveMaze_rec(maze_t *p_playground, list_t **p_visitedNodes, int current_line, int current_col)
{
    assert(p_playground != NULL);
    assert(*p_visitedNodes != NULL);

    // make sure that we are not into a wall
    if (p_playground->grid[current_line][current_col] == 0)
        return;

    int visitedNodesCount = getLength(*p_visitedNodes);

    // make sure that the current point is not on the track record
    int *current_track_record = listToArray(*p_visitedNodes, visitedNodesCount);
    bool is_current_tracked = liniar_search_array(current_track_record, visitedNodesCount, current_line * p_playground->col_count + current_col);

    if (is_current_tracked && visitedNodesCount > 1)
    {
        free(current_track_record);
        return;
    }
    else
    {
        free(current_track_record);
    }

    *p_visitedNodes = push_list(*p_visitedNodes, current_line * p_playground->col_count + current_col);
    visitedNodesCount++;

    // make sure that we are not at the end
    if (current_line == p_playground->end[0] && current_col == p_playground->end[1])
        return;

    list_t *visitedNodesCopy;
    int *array;
    int copySize;

    if (current_line + 1 < p_playground->row_count)
    {
        visitedNodesCopy = copy_list(*p_visitedNodes);
        solveMaze_rec(p_playground, &visitedNodesCopy, current_line + 1, current_col);

        copySize = getLength(visitedNodesCopy);
        array = listToArray(visitedNodesCopy, copySize);

        bool endReached = liniar_search_array(array, copySize, p_playground->end[0] * p_playground->col_count + p_playground->end[1]);

        // destroy the array to prevent memory leaks
        free(array);
        array = NULL;

        if (endReached)
        {

            // destroy the previous *p_visitedNodes list to prevent memory leaks
            destroy_list(*p_visitedNodes);
            *p_visitedNodes = NULL;

            // change the *p_visitedNodes list to the copy, since this one contains the path to the end
            *p_visitedNodes = visitedNodesCopy;
            return;
        }
        else
        {
            // destroy the copy if the end is not reached, it will be recreated at the next if statement
            destroy_list(visitedNodesCopy);
            visitedNodesCopy = NULL;
        }
    }

    if (current_col + 1 < p_playground->col_count)
    {
        visitedNodesCopy = copy_list(*p_visitedNodes);
        solveMaze_rec(p_playground, &visitedNodesCopy, current_line, current_col + 1);

        copySize = getLength(visitedNodesCopy);
        array = listToArray(visitedNodesCopy, copySize);

        bool endReached = liniar_search_array(array, copySize, p_playground->end[0] * p_playground->col_count + p_playground->end[1]);

        // destroy the array to prevent memory leaks
        free(array);
        array = NULL;

        if (endReached)
        {

            // destroy the previous *p_visitedNodes list to prevent memory leaks
            destroy_list(*p_visitedNodes);
            *p_visitedNodes = NULL;

            // change the *p_visitedNodes list to the copy, since this one contains the path to the end
            *p_visitedNodes = visitedNodesCopy;
            return;
        }
        else
        {
            // destroy the copy if the end is not reached, it will be recreated at the next if statement
            destroy_list(visitedNodesCopy);
            visitedNodesCopy = NULL;
        }
    }

    if (current_line - 1 >= 0)
    {
        visitedNodesCopy = copy_list(*p_visitedNodes);
        solveMaze_rec(p_playground, &visitedNodesCopy, current_line - 1, current_col);

        copySize = getLength(visitedNodesCopy);
        array = listToArray(visitedNodesCopy, copySize);

        bool endReached = liniar_search_array(array, copySize, p_playground->end[0] * p_playground->col_count + p_playground->end[1]);

        // destroy the array to prevent memory leaks
        free(array);
        array = NULL;

        if (endReached)
        {

            // destroy the previous *p_visitedNodes list to prevent memory leaks
            destroy_list(*p_visitedNodes);
            *p_visitedNodes = NULL;

            // change the *p_visitedNodes list to the copy, since this one contains the path to the end
            *p_visitedNodes = visitedNodesCopy;
            return;
        }
        else
        {
            // destroy the copy if the end is not reached, it will be recreated at the next if statement
            destroy_list(visitedNodesCopy);
            visitedNodesCopy = NULL;
        }
    }

    if (current_col - 1 >= 0)
    {
        visitedNodesCopy = copy_list(*p_visitedNodes);
        solveMaze_rec(p_playground, &visitedNodesCopy, current_line, current_col - 1);

        copySize = getLength(visitedNodesCopy);
        array = listToArray(visitedNodesCopy, copySize);

        bool endReached = liniar_search_array(array, copySize, p_playground->end[0] * p_playground->col_count + p_playground->end[1]);

        // destroy the array to prevent memory leaks
        free(array);
        array = NULL;

        if (endReached)
        {

            // destroy the previous *p_visitedNodes list to prevent memory leaks
            destroy_list(*p_visitedNodes);
            *p_visitedNodes = NULL;

            // change the *p_visitedNodes list to the copy, since this one contains the path to the end
            *p_visitedNodes = visitedNodesCopy;
            return;
        }
        else
        {
            // destroy the copy if the end is not reached, it will be recreated at the next if statement
            destroy_list(visitedNodesCopy);
            visitedNodesCopy = NULL;
        }
    }

    return;
}

void destroyMaze(maze_t *p_playground)
{
    assert(p_playground != NULL);

    destroyMatrix(&(p_playground->grid), p_playground->row_count, p_playground->col_count);
    return;
}