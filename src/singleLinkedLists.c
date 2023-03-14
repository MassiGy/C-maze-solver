#include "../headers/singleLinkedLists.h"

struct Node *create_node_list(int number)
{
    struct Node *item = malloc(sizeof(struct Node));

    assert(item != NULL);

    item->val = number;
    item->next = NULL;
    return item;
}

void print_list(struct Node *head)
{
    struct Node *walker = head;

    assert(walker != NULL);

    while (walker != NULL)
    {
        printf("%p -> %i\n", walker, walker->val);
        walker = walker->next;
    }
}

struct Node *push_list(struct Node *head, int number)
{
    assert(head != NULL);

    struct Node *walker = head;

    while (walker->next != NULL)
    {
        walker = walker->next;
    }

    walker->next = create_node_list(number);
    return head;
}

struct Node *pop_list(struct Node *head)
{

    assert(head != NULL);

    struct Node *walker = head;

    while (walker->next != NULL)
    {
        walker = walker->next;
    }

    return walker;
}

struct Node *unshift_list(struct Node *head, int number)
{
    assert(head != NULL);

    struct Node *item = create_node_list(number);

    assert(item != NULL);

    item->next = head;
    head = item;
    return head;
}

struct Node *shift_list(struct Node *head)
{
    assert(head != NULL);

    head = head->next;
    return head;
}

struct Node *destroy_list(struct Node *head)
{
    assert(head != NULL);

    struct Node *temp;

    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }

    return head;
}

struct Node *insertAt_list(struct Node *head, int pos, int number)
{
    assert(head != NULL);

    if (pos <= 0)
    {
        head = unshift_list(head, number);
        return head;
    }

    struct Node *walker = head;
    int counter = 0;

    while (walker->next != NULL && counter < pos)
    {
        walker = walker->next;
        counter++;
    }

    assert(walker != NULL);

    struct Node *temp = create_node_list(number);

    temp->next = walker->next;

    walker->next = temp;

    return head;
}

struct Node *splice_list(struct Node *head, int pos, int deleteCount)
{
    assert(head != NULL);
    int counter;

    if (pos <= 0)
    {
        counter = 0;
        while (counter < deleteCount)
        {
            head = shift_list(head);
            counter++;
        }

        return head;
    }

    struct Node *walker = head;

    counter = 0;
    while (walker != NULL && counter < pos)
    {
        walker = walker->next;
        counter++;
    }

    assert(walker != NULL);

    struct Node *temp = walker;
    counter = 0;

    while (temp != NULL && counter <= deleteCount)
    {
        temp = temp->next;
        counter++;
    }

    assert(temp != NULL);

    // we need to to free the elementsToRemove

    struct Node *elementToFree;

    while (walker->next != temp)
    {
        elementToFree = walker->next;
        walker->next = walker->next->next;
        free(elementToFree);
    }
    return head;
}

struct Node *updateAt_list(struct Node *head, int pos, int newVal)
{
    assert(head != NULL);

    if (pos <= 0)
    {
        head->val = newVal;
        return head;
    }

    struct Node *walker = head;
    int counter = 0;

    while (walker != NULL && counter < pos)
    {
        walker = walker->next;
        counter++;
    }

    assert(walker != NULL);

    walker->val = newVal;

    return head;
}

struct Node *split_list(struct Node *head, int pos, struct Node *rest)
{
    assert(head != NULL);

    struct Node *walker = head;

    int counter = 1;

    while (walker != NULL && counter < pos)
    {
        walker = walker->next;
        counter++;
    }

    assert(walker != NULL);

    rest = walker->next;
    walker->next = NULL;
    return rest;
}

void bubble_sort_list(struct Node *head, int length)
{
    assert(head != NULL);
    assert(length >= 2);

    int i = 0;
    int j = 0;
    bool swaps = true;
    struct Node *current = head;

    while (i < length - 1 && swaps)
    {
        j = 0;
        current = head;
        swaps = false;

        while (j < length - i - 1)
        {
            if (current->next->val < current->val)
            {
                // swap
                int temp = current->val;
                current->val = current->next->val;
                current->next->val = temp;

                swaps = true;
            }

            current = current->next;
            j++;
        }
        i++;
    }
}

int *listToArray(struct Node *head, int length)
{
    assert(head != NULL);
    assert(length > 0);

    int *res = malloc(length * sizeof(int));

    assert(res != NULL);

    struct Node *traversal = head;
    int counter = 0;

    while (counter < length)
    {
        /* code */
        res[counter] = traversal->val;
        traversal = traversal->next;

        counter++;
    }

    return res;
}

struct Node *arrayToList(int *arr, int length)
{
    assert(arr != NULL);
    assert(length > 0);

    struct Node *list = create_node_list(*(arr + length - 1));
    int counter = length - 2;

    while (counter >= 0)
    {
        // unshift is 0(1), so it is better then push ~ O(N)
        list = unshift_list(list, *(arr + counter));
        counter--;
    }

    return list;
}