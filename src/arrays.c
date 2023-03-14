#include "../headers/arrays.h"

void init_array(int *head, int size)
{
    for (int i = 0; i < size; ++i)
    {
        head[i] = 0;
    }
}

void print_array(int *head, int size)
{
    for (int i = 0; i < size; ++i)
    {
        printf("@%i: %i\n", i, head[i]);
    }
}

void fill_array(int *head, int size)
{
    for (int i = 0; i < size; ++i)
    {
        *(head + i) = (int)rand() % 100;
    }
}

int get_max_array(int *head, int size)
{
    int max = *head;

    for (int i = 0; i < size; ++i)
    {
        if (*(head + i) > max)
        {
            max = *(head + i);
        }
    }

    return max;
}

float get_average_array(int *head, int size)
{
    int sum = 0;

    for (int i = 0; i < size; ++i)
    {
        sum += *(head + i);
    }

    return (float)sum / size;
}

bool liniar_search_array(int *head, int size, int val)
{
    bool res = false;
    for (int i = 0; i < size; ++i)
    {
        if (*(head + i) == val)
        {
            res = true;
            break;
        }
    }
    return res;
}

bool binary_search_array(int *head, int first, int last, int val)
{
    bool resault = false;

    if (val < head[0] || val > head[last])
    {
        return resault;
    }

    if (head[first] == val || head[last] == val)
    {
        resault = true;
        return resault;
    }

    if (first == last)
    {
        return resault;
    }

    int mid = (first + last) / 2;

    if (head[mid] > val)
    {
        resault = binary_search_array(head, first, mid - 1, val);
    }
    else
    {
        resault = binary_search_array(head, mid + 1, last, val);
    }

    return resault;
}

void bubble_sort_array(int *head, int size)
{
    if (size <= 1)
        return;

    for (int i = 0; i < size - 1; ++i)
    {
        for (int j = i + 1; j < size; ++j)
        {
            if (*(head + j) < *(head + i))
            {
                // swap
                int temp = head[j];
                head[j] = head[i];
                head[i] = temp;
            }
        }
    }
}

void selection_sort_array(int *head, int size)
{
    assert(head != NULL);
    assert(size > 1);

    // initiate the max index ;
    int maxIndex = 0;

    for (int i = 0; i < size; ++i)
    {
        int j;
        for (j = 0; j < size - i - 1; ++j)
        {
            if (*(head + j) > *(head + maxIndex))
            {
                maxIndex = j;
            }
        }

        // swap the max and the last element value

        int temp = *(head + j);
        *(head + j) = *(head + maxIndex);
        *(head + maxIndex) = temp;
    }
}

void insertion_sort_array(int *head, int size)
{
    assert(head != NULL);
    assert(size > 1);

    // set the last element index of the sub sorted array
    int sub_sorted_arr_lastEl_index = 0;

    // walkthrough the rest of the array starting from the sub sorted arr lastEl index
    for (int i = 1; i < size; ++i)
    {
        if (head[i] < head[sub_sorted_arr_lastEl_index])
        {
            // go through the sub sorted array to find where you should insert the element
            for (int j = 0; j <= sub_sorted_arr_lastEl_index; ++j)
            {
                if (head[j] > head[i])
                {
                    // save the head[i] val to not override it
                    int temp = head[i];

                    // shift the array elements from j+1 to length - 1
                    for (int k = sub_sorted_arr_lastEl_index + 1; k > j; --k)
                    {
                        head[k] = head[k - 1];
                    }

                    // put the temp val in the head[sub_sorted_lastEl_index] and increment this refrence
                    // CAUTION: we can only override the j'th element, only if this one is duplicated in the j+1'th pos
                    // so we can only do this, if the operation head[j+1] = head[j] occures on the loop before
                    head[j] = temp;
                    sub_sorted_arr_lastEl_index++;
                }
            }
        }
        else
        {
            sub_sorted_arr_lastEl_index++;
        }
    }
}

/*
    This version of the insertion sort algo uses another strategy compared to the last one.
    This will insert always at the position of the last sub sorted array index,
    whereas the one above always inserts before the last sub sorted array element,
    meaning the it will always insert into the sub sorted array.

    Notice that :
        - first one is ~ O(N*M*P)
            | N nb elmes of main array, M nb elems sub array, P nb elems between element to insert and sub array last element
        - second one is ~ O(N*(N-k)) k:[1..N-1]
*/

void q_insertion_sort_array(int *head, int size)
{
    assert(head != NULL);
    assert(size > 1);

    // keep track of the sub sorted array logical size - 1
    int sub_sorted_array_lastEL_index = 0;

    // walkthrough the rest of the array
    for (int i = 1; i < size; ++i)
    {
        if (head[i] < head[sub_sorted_array_lastEL_index])
        {
            for (int j = 0; j < sub_sorted_array_lastEL_index + 1; ++j)
            {
                if (head[j] > head[i])
                {
                    // insert the element
                    int temp = head[i];
                    head[i] = head[j];
                    head[j] = temp;
                }
            }
            sub_sorted_array_lastEL_index++;
        }
    }
}

/*
    Binary insertion sort is a more optemized version of the insertion_sort
    The idea is simple, since the sub array is sorted we can binary traverse it.

    In this case, the binary traversal will search for the first element on the sub sorted array
    that is bigger then the element to insert from the rest of the main array.

    This will make our time complexity ~ O(N * k *log(K)) where K:[1..N]
*/

void binary_insertion_sort_array(int *head, int size)
{
    assert(head != NULL);
    assert(size > 1);

    // keep track of the last elemnent on the sub sorted array
    int sub_sorted_array_last_index = 0;

    // walkthrough the rest of the array;
    for (int i = 1; i < size; ++i)
    {
        if (head[i] < head[sub_sorted_array_last_index])
        {
            // left will store the start of the sub sorted array
            int left = 0;
            // right will store the end of the sub sorted array
            int right = sub_sorted_array_last_index;

            while (left < right)
            {
                /* code */
                int mid = (left + right) / 2;

                if (head[i] <= head[mid])
                {
                    right = mid;
                }
                else
                {
                    left = mid + 1;
                }
            }

            // now left is the index of the first element
            // on the sub sorted array that is bigger then head[i]

            // now we need to shift the sub sorted array by one position to the right

            // first store head[i] before the override
            int temp = head[i];

            for (int j = i; j > left; --j)
            {
                head[j] = head[j - 1];
            }

            // insert the element head[i] into the index = val(left)

            head[left] = temp;
        }
        sub_sorted_array_last_index++;
    }
}

void merge_sort_array(int **head, int start, int end)
{
    assert(*head != NULL);

    if (end - start <= 0)
        return;

    int mid = (start + end) / 2;

    merge_sort_array(head, start, mid);
    merge_sort_array(head, mid + 1, end);

    merge_array(head, start, mid, end);
}

/*
    This merge function need to be debugged
    The problem is on the sorting
    Consider using two sub arrays to store a local version of left to mid and mid+1 to end
*/
void merge_array(int **head, int start, int mid, int end)
{
    assert(*head != NULL);
    assert(end > start);

    int head_walker = start;
    int left_walker = 0;
    int right_walker = 0;

    int left[mid - start + 1];
    int right[end - mid];

    // copy the first half of the array into left
    for (int i = 0; i < (mid - start +1); ++i)
    {
        left[i] = (*head)[i + start];
    }

    // copy the second half of the array into right
    for (int i = 0; i < (end - mid); ++i)
    {
        right[i] = (*head)[i + mid + 1];
    }

    // merge the left and the right subarrays into the head array
    while (left_walker < (mid - start + 1) && right_walker < (end - mid))
    {
        /* code */
        if (left[left_walker] <= right[right_walker])
        {
            (*head)[head_walker] = left[left_walker];
            left_walker++;
        }
        else
        {
            (*head)[head_walker] = right[right_walker];
            right_walker++;
        }
        head_walker++;
    }

    /* Copy the remaining elements of left[], if there
   are any */
    while (left_walker < (mid - start + 1))
    {
        (*head)[head_walker] = left[left_walker];
        left_walker++;
        head_walker++;
    }

    /* Copy the remaining elements of right[], if there
    are any */
    while (right_walker < (end - mid))
    {
        (*head)[head_walker] = right[right_walker];
        right_walker++;
        head_walker++;
    }
}