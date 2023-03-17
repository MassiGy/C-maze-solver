// main.c source code 
#include "../headers/main.h"


int main(void)
{
    maze_t maze;
    loadMaze(&maze);
    printMatrix(&(maze.grid), maze.row_count,maze.col_count);
    findStart(&maze);
    findEnd(&maze);


    printf("Entrée : ");
    for (int i = 0; i < 2; i++)
    {
        printf("[%d]", maze.entry[i]);
    }

    printf("\nSortie : ");
    for (int i = 0; i < 2; i++)
    {
        printf("[%d]", maze.end[i]);
    }
    
    printf("\n");

    // int current_line = get_line(maze.entry[0] * maze.col_count + maze.entry[1],maze.col_count);
    // int current_col = get_colomn(maze.entry[0] * maze.col_count + maze.entry[1], maze.col_count);
    // printf("current_line = %d, current_col  = %d\n", current_line,current_col);

    // printf("can_go_up = %d\n", can_go_up(current_line,current_col,&maze,maze.entry[0] * maze.col_count + maze.entry[1],maze.entry[0] * maze.col_count + maze.entry[1] + maze.col_count));
    // printf("can_go_down = %d\n", can_go_down(current_line,current_col,&maze,maze.entry[0] * maze.col_count + maze.entry[1],maze.entry[0] * maze.col_count + maze.entry[1] + maze.col_count));
    // printf("can_go_left = %d\n", can_go_left(current_line,current_col,&maze,maze.entry[0] * maze.col_count + maze.entry[1],maze.entry[0] * maze.col_count + maze.entry[1] + maze.col_count));
    // printf("can_go_right = %d\n", can_go_right(current_line,current_col,&maze,maze.entry[0] * maze.col_count + maze.entry[1],maze.entry[0] * maze.col_count + maze.entry[1] + maze.col_count));





    // return;

    checkpoint_t start_checkpoint;
    start_checkpoint.current_track_record = create_node_list(maze.entry[0] * maze.col_count + maze.entry[1]);
    start_checkpoint.direction = (1);
    start_checkpoint.end_reached = false;
    start_checkpoint.p_maze = &maze;
    start_checkpoint.last_pos = maze.entry[0] * maze.col_count + maze.entry[1];


    


    solveMaze_threaded(&start_checkpoint);


    if (getLength(start_checkpoint.current_track_record) >= 1)
    {
        print_list(start_checkpoint.current_track_record);
        start_checkpoint.current_track_record = destroy_list(start_checkpoint.current_track_record);
    }


    destroyMaze(&maze);

    return 0;
}