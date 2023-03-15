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
bool can_go_up(int current_line, int current_col, maze_t *maze, int last_pos, int next_move)
{
    return current_line - 1 >= 0 && maze->grid[current_line - 1][current_col] != 0 && (current_line - 1) * maze->col_count + maze->col_count != last_pos && (current_line - 1) * maze->col_count + maze->col_count != next_move;
}
bool can_go_down(int current_line, int current_col, maze_t *maze, int last_pos, int next_move)
{
    return current_line + 1 < maze->row_count && maze->grid[current_line + 1][current_col] != 0 && (current_line + 1) * maze->col_count + maze->col_count != last_pos && (current_line + 1) * maze->col_count + maze->col_count != next_move;
}
bool can_go_right(int current_line, int current_col, maze_t *maze, int last_pos, int next_move)
{
    return current_col + 1 < maze->col_count && maze->grid[current_line][current_col + 1] != 0 && current_line * maze->col_count + maze->col_count + 1 != last_pos && current_line * maze->col_count + maze->col_count + 1 != next_move;
}
bool can_go_left(int current_line, int current_col, maze_t *maze, int last_pos, int next_move)
{
    return current_col - 1 >= 0 && maze->grid[current_line][current_col - 1] != 0 && current_line * maze->col_count + maze->col_count - 1 != last_pos && current_line * maze->col_count + maze->col_count - 1 != next_move;
}

void *solveMaze_threaded(void *checkpoint)
{
    assert(checkpoint != NULL);

    checkpoint_t *current_checkpoint = (checkpoint_t *)checkpoint;
    int current_line = get_line(current_checkpoint->last_pos, maze.col_count);
    int current_col = get_colomn(current_checkpoint->last_pos, maze.col_count);

    // make sure that the parent is on its track record
    assert(current_checkpoint->current_track_record != NULL);

    // check if the end is reached
    if (current_checkpoint->last_pos == maze.end[0] * maze.col_count + maze.end[1])
    {
        return NULL;
    }

    int next_move;
    bool is_up_possible;
    bool is_down_possible;
    bool is_left_possible;
    bool is_right_possible;
    pthread_t up_thread, down_thread, left_thread, right_thread;
    checkpoint_t up_thread_checkpoint, down_thread_checkpoint, left_thread_checkpoint, right_thread_checkpoint;

    int *constructed_path;
    int constructed_path_lenght;
    bool does_lead_to_end;

    while (!(current_checkpoint->end_reached))
    {
        current_line = get_line(current_checkpoint->last_pos, maze.col_count);
        current_col = get_colomn(current_checkpoint->last_pos, maze.col_count);

        /*check for any other possible ways*/
        next_move = current_checkpoint->last_pos + current_checkpoint->direction;
        is_up_possible = can_go_up(current_line, current_col, &maze, current_checkpoint->last_pos, next_move);
        is_down_possible = can_go_down(current_line, current_col, &maze, current_checkpoint->last_pos, next_move);
        is_left_possible = can_go_left(current_line, current_col, &maze, current_checkpoint->last_pos, next_move);
        is_right_possible = can_go_right(current_line, current_col, &maze, current_checkpoint->last_pos, next_move);

        /* if there is any way, fire up a thread following it */
        if (is_up_possible)
        {

            // create a thread to go up

            /* make sure that the parent<>son relation is set trought out the list_t * possible_way[4] */
            current_checkpoint->possible_ways[0] = copy_list(current_checkpoint->current_track_record);
            up_thread_checkpoint.current_track_record = current_checkpoint->possible_ways[0];

            /* make sure that the passed direction is the correct one */
            up_thread_checkpoint.direction = -(maze.col_count);
            /* make sure that the passed last_position is actually the current position */
            up_thread_checkpoint.last_pos = current_line * maze.col_count + current_col;
            /* make sure that the son also goes to search for the end */
            up_thread_checkpoint.end_reached = current_checkpoint->end_reached;

            pthread_create(&up_thread, NULL, &solveMaze_threaded, &up_thread_checkpoint);
        }

        if (is_down_possible)
        {
            // create a thread to go down

            /* make sure that the parent<>son relation is set trought out the list_t * possible_way[4] */
            current_checkpoint->possible_ways[1] = copy_list(current_checkpoint->current_track_record);
            down_thread_checkpoint.current_track_record = current_checkpoint->possible_ways[1];

            /* make sure that the passed direction is the correct one */
            down_thread_checkpoint.direction = +(maze.col_count);
            /* make sure that the passed last_position is actually the current position */
            down_thread_checkpoint.last_pos = current_line * maze.col_count + current_col;
            /* make sure that the son also goes to search for the end */
            down_thread_checkpoint.end_reached = current_checkpoint->end_reached;

            pthread_create(&down_thread, NULL, &solveMaze_threaded, &down_thread_checkpoint);
        }
        if (is_left_possible)
        {
            // create a thread to go left

            /* make sure that the parent<>son relation is set trought out the list_t * possible_way[4] */
            current_checkpoint->possible_ways[2] = copy_list(current_checkpoint->current_track_record);
            left_thread_checkpoint.current_track_record = current_checkpoint->possible_ways[2];

            /* make sure that the passed direction is the correct one */
            left_thread_checkpoint.direction = -(1);
            /* make sure that the passed last_position is actually the current position */
            left_thread_checkpoint.last_pos = current_line * maze.col_count + current_col;
            /* make sure that the son also goes to search for the end */
            left_thread_checkpoint.end_reached = current_checkpoint->end_reached;

            pthread_create(&left_thread, NULL, &solveMaze_threaded, &left_thread_checkpoint);
        }
        if (is_right_possible)
        {
            // create a thread to go right

            /* make sure that the parent<>son relation is set trought out the list_t * possible_way[4] */
            current_checkpoint->possible_ways[3] = copy_list(current_checkpoint->current_track_record);
            right_thread_checkpoint.current_track_record = current_checkpoint->possible_ways[3];

            /* make sure that the passed direction is the correct one */
            right_thread_checkpoint.direction = (1);
            /* make sure that the passed last_position is actually the current position */
            right_thread_checkpoint.last_pos = current_line * maze.col_count + current_col;
            /* make sure that the son also goes to search for the end */
            right_thread_checkpoint.end_reached = current_checkpoint->end_reached;

            pthread_create(&right_thread, NULL, &solveMaze_threaded, &right_thread_checkpoint);
        }

        /* if any thread gets spawned, we will wait for it */
        if (is_up_possible)
        {
            pthread_join(up_thread, NULL);
            /* see if the constructed path by this thread leads to the end */
            constructed_path_lenght = getLength(current_checkpoint->possible_ways[0]);
            constructed_path = listToArray(current_checkpoint->possible_ways[0], constructed_path_lenght);

            does_lead_to_end = liniar_search_array(constructed_path, constructed_path_lenght, maze.end[0] * maze.col_count + maze.end[1]);

            if (does_lead_to_end)
            {
                /* replace the parent track record with the son track record */
                current_checkpoint->current_track_record = current_checkpoint->possible_ways[0];
                current_checkpoint->direction = 0;
                current_checkpoint->end_reached = true;
                current_checkpoint->last_pos = maze.end[0] * maze.col_count + maze.end[1];

                /*ignore the following and break out from the loop */
                break;
            }
            else
            {
                /* free the array */
                free(constructed_path);
            }
        }
        if (is_down_possible)
        {
            pthread_join(down_thread, NULL);
            /* see if the constructed path by this thread leads to the end */
            constructed_path_lenght = getLength(current_checkpoint->possible_ways[1]);
            constructed_path = listToArray(current_checkpoint->possible_ways[1], constructed_path_lenght);

            does_lead_to_end = liniar_search_array(constructed_path, constructed_path_lenght, maze.end[0] * maze.col_count + maze.end[1]);

            if (does_lead_to_end)
            {
                /* replace the parent track record with the son track record */
                current_checkpoint->current_track_record = current_checkpoint->possible_ways[1];
                current_checkpoint->direction = 0;
                current_checkpoint->end_reached = true;
                current_checkpoint->last_pos = maze.end[0] * maze.col_count + maze.end[1];

                /*ignore the following and break out from the loop */
                break;
            }
            else
            {
                /* free the array */
                free(constructed_path);
            }
        }
        if (is_left_possible)
        {
            pthread_join(left_thread, NULL);

            /* see if the constructed path by this thread leads to the end */
            constructed_path_lenght = getLength(current_checkpoint->possible_ways[2]);
            constructed_path = listToArray(current_checkpoint->possible_ways[2], constructed_path_lenght);

            does_lead_to_end = liniar_search_array(constructed_path, constructed_path_lenght, maze.end[0] * maze.col_count + maze.end[1]);

            if (does_lead_to_end)
            {
                /* replace the parent track record with the son track record */
                current_checkpoint->current_track_record = current_checkpoint->possible_ways[2];
                current_checkpoint->direction = 0;
                current_checkpoint->end_reached = true;
                current_checkpoint->last_pos = maze.end[0] * maze.col_count + maze.end[1];

                /*ignore the following and break out from the loop */
                break;
            }
            else
            {
                /* free the array */
                free(constructed_path);
            }
        }
        if (is_right_possible)
        {
            pthread_join(right_thread, NULL);

            /* see if the constructed path by this thread leads to the end */
            constructed_path_lenght = getLength(current_checkpoint->possible_ways[3]);
            constructed_path = listToArray(current_checkpoint->possible_ways[3], constructed_path_lenght);

            does_lead_to_end = liniar_search_array(constructed_path, constructed_path_lenght, maze.end[0] * maze.col_count + maze.end[1]);

            if (does_lead_to_end)
            {
                /* replace the parent track record with the son track record */
                current_checkpoint->current_track_record = current_checkpoint->possible_ways[3];
                current_checkpoint->direction = 0;
                current_checkpoint->end_reached = true;
                current_checkpoint->last_pos = maze.end[0] * maze.col_count + maze.end[1];

                /*ignore the following and break out from the loop */
                break;
            }
            else
            {
                /* free the array */
                free(constructed_path);
            }
        }

        /* then just continue moving according to the specified direction */
        current_checkpoint->last_pos = next_move;
        current_checkpoint->current_track_record = push_list(current_checkpoint->current_track_record, next_move);
        current_checkpoint->end_reached = current_checkpoint->last_pos == maze.end[0] * maze.col_count + maze.end[1];
    }

    return NULL;
}

void solveMaze_rec(maze_t *p_playground, list_t **p_visitedNodes, int current_line, int current_col)
{
    assert(p_playground != NULL);
    assert(*p_visitedNodes != NULL);

    printf("current_line = %d, current_col = %d\n", current_line, current_col);

    // make sure that we are not into a wall
    if (p_playground->grid[current_line][current_col] == 0)
        return;

    int visitedNodesCount = getLength(*p_visitedNodes);
    // make sure that we are not at the entry again.
    if (p_playground->grid[current_line][current_col] == 2 && visitedNodesCount > 1)
        return;

    // make sure that the current point is not on the track record
    int *current_track_record = listToArray(*p_visitedNodes, visitedNodesCount);
    bool is_current_tracked = liniar_search_array(current_track_record, visitedNodesCount, current_line * p_playground->col_count + current_col);

    if (is_current_tracked && visitedNodesCount > 1)
    {
        free(current_track_record);
        return;
    }
    else
        free(current_track_record);

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