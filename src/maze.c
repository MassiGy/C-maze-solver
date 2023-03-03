#include "../headers/maze.h"

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

    char buffer[playground->col_count];

    for (int i = 0; i < playground->row_count; i++)
    {
        /* read a line*/
        fscanf(_file, "%s", &buffer);

        for (int j = 0; j < playground->col_count; j++)
        {
            /* read the int and load them to the matrix */
            playground->grid[i][j] = atoi(buffer[i]);
        }
    }
    return;
}

void solveMaze(maze_t *playground);

void destroyMaze(maze_t **p_playground)
{
    assert(p_playground != NULL);
    assert(*p_playground != NULL);

    destroyMatrix(&((*p_playground)->grid), (*p_playground)->row_count, (*p_playground)->col_count);

    free(*p_playground);
    *p_playground = NULL;

    return;
}