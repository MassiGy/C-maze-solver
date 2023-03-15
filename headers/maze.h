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


struct checkpoint {
    list_t * current_track_record;
    int direction;
    int last_pos;
    bool end_reached;
    list_t * possible_ways[4];
};



typedef struct maze maze_t;
typedef struct checkpoint checkpoint_t;


maze_t maze;




void findStart(maze_t *playground);
void findEnd(maze_t *playground);
void loadMaze(maze_t *p_playground);
void solveMaze(maze_t *p_playground);
void solveMaze_rec(maze_t *p_playground, list_t **p_visitedNodes, int current_line, int current_col);
void destroyMaze(maze_t *p_playground);
