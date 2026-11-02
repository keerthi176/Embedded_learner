// Q 9. Given the code below, what is the output of matrix[0][1] and matrix[1][0] after the dynamic re-indexing operation?

#include "stdio.h"
#include "stdlib.h"

void swap_rows(int **mat, int r1, int r2)
{
    int *temp = mat[r1];
    mat[r1]   = mat[r2];
    mat[r2]   = temp;
}

int main(void)
{
    int **mat = malloc(2 * sizeof(int *));

    mat[0] = (int[]){10, 20, 30};
    mat[1] = (int[]){40, 50, 60};

    swap_rows(mat, 0, 1);

    int **p = mat;

    printf("%d %d\n", p[0][1], p[1][0]);

    free(mat);

    return 0;
}

// Answer: The program prints "50 10".
