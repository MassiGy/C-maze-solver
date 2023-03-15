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

void solveMaze(maze_t *p_playground)
{
}

void solveMaze_rec(maze_t *p_playground, list_t **p_visitedNodes, int visitedNodesCount, int current_line, int current_col)
{
    assert(p_playground != NULL);
    assert(*p_visitedNodes != NULL);

    /* see if the currrent position is blocked */
    printf("current_line = %d, current_col = %d\n", current_line, current_col);
    if (p_playground->grid[current_line][current_col] == 0)
        return;

    /*first make sure that the visitedNodes list does not aleady contain the end*/
    int *array = listToArray(*p_visitedNodes, visitedNodesCount);

    if (liniar_search_array(array, visitedNodesCount, p_playground->end[0] * p_playground->col_count + p_playground->end[1]))
        return;

    /* second, make sure that the actual node is not already in the list*/
    if (
        current_line * p_playground->col_count + current_col != p_playground->entry[0] * p_playground->col_count + p_playground->entry[1] &&
        liniar_search_array(array, visitedNodesCount, current_line * p_playground->col_count + current_col))
    {
        return;
    }

    *p_visitedNodes = push_list(*p_visitedNodes, current_line * p_playground->col_count + current_col);

    if (current_line == p_playground->end[0] && current_col == p_playground->end[1])
        return;

    list_t *visitedNodesCopy;
    int copySize;

    if (current_line + 1 <= p_playground->row_count)
    {
        visitedNodesCopy = copy_list(*p_visitedNodes);
        solveMaze_rec(p_playground, &visitedNodesCopy, visitedNodesCount + 1, current_line + 1, current_col);

        /* check if the modified copy containes the end */
        copySize = getLength(visitedNodesCopy);
        array = listToArray(visitedNodesCopy, copySize);

        if (liniar_search_array(array, copySize, p_playground->end[0] * p_playground->col_count + p_playground->end[1]))
        {
            p_visitedNodes = &visitedNodesCopy;
            return;
        }
    }

    if (current_line - 1 >= 0)
    {

        visitedNodesCopy = copy_list(*p_visitedNodes);
        solveMaze_rec(p_playground, &visitedNodesCopy, visitedNodesCount + 1, current_line - 1, current_col);

        /* check if the modified copy containes the end */
        copySize = getLength(visitedNodesCopy);
        array = listToArray(visitedNodesCopy, copySize);

        if (liniar_search_array(array, copySize, p_playground->end[0] * p_playground->col_count + p_playground->end[1]))
        {
            p_visitedNodes = &visitedNodesCopy;
            return;
        }
    }

    if (current_col + 1 <= p_playground->col_count)
    {

        visitedNodesCopy = copy_list(*p_visitedNodes);
        solveMaze_rec(p_playground, &visitedNodesCopy, visitedNodesCount + 1, current_line + 1, current_col);

        /* check if the modified copy containes the end */
        copySize = getLength(visitedNodesCopy);
        array = listToArray(visitedNodesCopy, copySize);

        if (liniar_search_array(array, copySize, p_playground->end[0] * p_playground->col_count + p_playground->end[1]))
        {
            p_visitedNodes = &visitedNodesCopy;
            return;
        }
    }

    if (current_col - 1 >= 0)
    {

        visitedNodesCopy = copy_list(*p_visitedNodes);
        solveMaze_rec(p_playground, &visitedNodesCopy, visitedNodesCount + 1, current_line - 1, current_col);

        /* check if the modified copy containes the end */
        copySize = getLength(visitedNodesCopy);
        array = listToArray(visitedNodesCopy, copySize);

        if (liniar_search_array(array, copySize, p_playground->end[0] * p_playground->col_count + p_playground->end[1]))
        {
            p_visitedNodes = &visitedNodesCopy;
            return;
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