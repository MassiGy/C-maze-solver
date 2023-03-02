#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

int **createMatrix(int row_count, int col_count);
void destroyMatrix(int ***p_matrix, int row_count, int col_count);
void initRandomMatrix(int ***p_matrix, int row_count, int col_count);
void printMatrix(int ***p_matrix, int row_count, int col_count);
int **inverseMatrix(int ***p_matrix, int row_count, int col_count);
