#include "../headers/matrixes.h"

int **createMatrix(int row_count, int col_count)
{
    int **res = malloc(row_count * sizeof(int *));

    for (int i = 0; i < row_count; ++i)
    {
        res[i] = malloc( col_count* sizeof(int));
    }

    return res;
}

void destroyMatrix(int ***p_matrix,int row_count, int col_count)
{
    assert(p_matrix != NULL);
    assert(*p_matrix != NULL);

    // first free each array
    for (int i = 0; i < row_count; ++i)
    {
        free((*p_matrix)[i]);
    }

    free(*p_matrix);
    p_matrix = NULL;
}

void initRandomMatrix(int ***p_matrix,int row_count, int col_count)
{
    assert(p_matrix != NULL);
    assert(*p_matrix != NULL);

    for (int i = 0; i < row_count; ++i)
    {
        for (int j = 0; j < col_count; ++j)
        {
            (*p_matrix)[i][j] = (int)(rand() / 10000000);
        }
    }
}

void printMatrix(int ***p_matrix,int row_count, int col_count)
{
    assert(p_matrix != NULL);
    assert(*p_matrix != NULL);

    for (int i = 0; i < row_count; ++i)
    {
        for (int j = 0; j < col_count; ++j)
        {
            printf("%i\t", (*p_matrix)[i][j]);
        }
        printf("\n");
    }
}

int **inverseMatrix(int ***p_matrix, int row_count, int col_count)
{
    assert(p_matrix != NULL);
    assert(*p_matrix != NULL);

    // create a new matrix which will be the inverse

    int **inverse = createMatrix(col_count,row_count);

    // fill the inverse

    for (int i = 0; i < row_count; ++i)
    {
        for (int j = 0; j < col_count; ++j)
        {
            inverse[i][j] = (*p_matrix)[j][i];
        }
    }
    return inverse;
}