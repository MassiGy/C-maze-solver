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

struct checkpoint
{
    list_t *current_track_record;
    int direction;
    int last_pos;
    bool end_reached;
    struct maze *p_maze;

    bool limited_threads;
    sem_t * free_threads_count;
};

typedef struct maze maze_t;
typedef struct checkpoint checkpoint_t;






void findStart(maze_t *playground);
void findEnd(maze_t *playground);
void loadMaze(maze_t *p_playground);
void solveMaze_rec(maze_t *p_playground, list_t **p_visitedNodes, int current_line, int current_col);
void *solveMaze_threaded(void *checkpoint);
void destroyMaze(maze_t *p_playground);

/** UTILS */
int get_line(int el_pos, int maze_col_count);
int get_colomn(int el_pos, int maze_col_count);

bool can_go_up(int current_line, int current_col, maze_t *maze, list_t *track_record);
bool can_go_down(int current_line, int current_col, maze_t *maze, list_t *track_record);
bool can_go_right(int current_line, int current_col, maze_t *maze, list_t *track_record);
bool can_go_left(int current_line, int current_col, maze_t *maze, list_t *track_record);

