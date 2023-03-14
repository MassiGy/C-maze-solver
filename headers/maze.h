#include "./matrixes.h"

struct maze
{
    int entry[2];
    int end[2];
    int row_count;
    int col_count;
    int **grid;
};

typedef struct maze maze_t;

void findStart(maze_t *playground);
void findEnd(maze_t *playground);
void loadMaze(maze_t *playground);
void solveMaze(maze_t *playground);
void destroyMaze(maze_t **playground);