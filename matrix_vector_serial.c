#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

#define SHAPE 10000
#define RANGE 10

int main(int argc)
{

    double wt1, wt2, wt3;

    srand(time(NULL));

    int(*A)[SHAPE] = malloc((sizeof(int)) * SHAPE * SHAPE);
    int(*x) = malloc((sizeof(int)) * SHAPE); // column vector x^T

    int(*res) = malloc((sizeof(int)) * SHAPE);

    wt1 = omp_get_wtime();

    for (int i = 0; i < SHAPE; i++)
    {
        x[i] = rand() % RANGE;
        for (int j = 0; j < SHAPE; j++)
        {
            A[i][j] = rand() % RANGE;
        }
    }

    wt2 = omp_get_wtime();

    for (int i = 0; i < SHAPE; i++)
    {
        int temp = 0;
        for (int j = 0; j < SHAPE; j++)
        {
            temp += A[i][j] * x[j];
        }
        res[i] = temp;
    }

    wt3 = omp_get_wtime();

    printf("Matrix A sample:\n");
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            printf("%d ", A[i][j]);
        }
        printf("\n");
    }

    printf("\nVector x sample:\n");
    for (int i = 0; i < 10; i++)
    {
        printf("%d\n", x[i]);
    }

    printf("\nResult sample:\n");
    for (int i = 0; i < 10; i++)
    {
        printf("%d\n", res[i]);
    }

    free(A);
    free(x);
    free(res);

    printf("\nMatrix & Vector initialization: %12.4g sec\n", wt2 - wt1);
    printf("\nMatrix & Vector multiplication: %12.4g sec\n", wt3 - wt2);
    printf("\nTotal time: %12.4g sec\n", wt3 - wt1);

    return 0;
}
