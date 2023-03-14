#include "./sys_headers.h"

struct Node
{
    /* data */
    int val;
    struct Node *next;
};


typedef struct Node list_t;
/*CRUD OPERATIONS*/

void print_list(struct Node *head);
int  getLength(struct Node * head);
struct Node *create_node_list(int number);
struct Node * copy_list(struct Node  * origin);
struct Node *push_list(struct Node *head, int number);
struct Node *pop_list(struct Node *head);
struct Node *unshift_list(struct Node *head, int number);
struct Node *shift_list(struct Node *head);
struct Node *insertAt_list(struct Node *head, int pos, int number);
struct Node *updateAt_list(struct Node *head, int pos, int newVal);
struct Node *splice_list(struct Node *head, int pos, int deleteCount);
struct Node *split_list(struct Node *head, int pos, struct Node *rest);
struct Node *destroy_list(struct Node *head);

// array to list
struct Node *arrayToList(int *arr, int length);
int *listToArray(struct Node *head,int length);

/* SORTING OPERATIONS */
void bubble_sort_list(struct Node *head, int length);


