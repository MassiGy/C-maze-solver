#include "./matrixes.h"
#include "./arrays.h"
#include "./singleLinkedLists.h"

struct maze
{
    int entry[2];
    int end[2];
    int row_count;
    int col_count;
    int **grid;
};

typedef struct maze maze_t;

void loadMaze(maze_t *p_playground);
void solveMaze(maze_t *p_playground);
void solveMaze_rec(maze_t *p_playground, list_t **p_visitedNodes, int current_line, int current_col);
void destroyMaze(maze_t *p_playground);