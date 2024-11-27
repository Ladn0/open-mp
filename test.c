#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define ARRAY_SIZE 1000

int main(void)
{
    // Declaring an array to hold random numbers of the size of ARRAY_SIZE.
    int array[ARRAY_SIZE];

    // Initializing the random number generator for the array
    srand(time(NULL));

    // Populating the array with random numbers
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        array[i] = rand() % 10; // Generating random number 1-10
    }

    // Printing the original array
    // printf("Original array: \n");
    // for (int i = 0; i < ARRAY_SIZE; i++)
    // {
    //     printf("%d ", array[i]);
    // }
    // printf("\n");

    double start_time = omp_get_wtime(); // Start timing
    // Implementing bubble sort
    for (int i = 0; i < ARRAY_SIZE - 1; i++)
    {
// Parallelizing the even-indexed comparisons
#pragma omp parallel for
        for (int j = 0; j < ARRAY_SIZE - i - 1; j += 2)
        {
            if (array[j] > array[j + 1])
            {
                int temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }

// Parallelizing the odd-indexed comparisons
#pragma omp parallel for
        for (int j = 1; j < ARRAY_SIZE - i - 1; j += 2)
        {
            if (array[j] > array[j + 1])
            {
                int temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
    double end_time = omp_get_wtime(); // End timing

    for (int i = 1; i < ARRAY_SIZE; i++)
    {
        if (array[i] >= array[i - 1])
        {
            continue;
        }
        else
        {
            printf("The sorting is wrong");
            break;
        }
    }

    // Printing the sorted array
    // printf("Sorted array: \n");
    // for (int i = 0; i < ARRAY_SIZE; i++)
    // {
    //     printf("%d ", array[i]);
    // }
    printf("\n");
    printf("Time taken (parallel): %f seconds\n", end_time - start_time);
}