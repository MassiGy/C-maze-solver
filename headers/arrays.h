#include "./sys_headers.h"

void init_array(int *head, int size);
void print_array(int *head, int size);
void fill_array(int *head, int size);
int get_max_array(int *head, int size);
float get_average_array(int *head, int size);

/*ADVANCED*/
bool liniar_search_array(int *head, int size, int val);
bool binary_search_array(int *head, int first, int last, int val);
void bubble_sort_array(int *head, int size);
void selection_sort_array(int *head, int size);
void insertion_sort_array(int *head, int size);
void q_insertion_sort_array(int *head, int size);
void binary_insertion_sort_array(int *head, int size);
void merge_sort_array(int **head, int start, int end);
void merge_array(int **head, int start, int mid, int end);
