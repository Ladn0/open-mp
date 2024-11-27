#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

#define INPUT_LENGTH 100000
#define THRESHOLD 1000
#define NUMBER_OF_THREADS 8

// CODE BY Artem Anufriyev

void mergeParallel(int *arr, int left, int mid, int right)
{
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int *leftArr = malloc((sizeof(int)) * n1);
    int *rightArr = malloc((sizeof(int)) * n2);

    for (i = 0; i < n1; i++)
        leftArr[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        rightArr[j] = arr[mid + 1 + j];

    i = 0;
    j = 0;
    k = left;
    while (i < n1 && j < n2)
    {
        if (leftArr[i] <= rightArr[j])
        {
            arr[k] = leftArr[i];
            i++;
        }
        else
        {
            arr[k] = rightArr[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        arr[k] = leftArr[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        arr[k] = rightArr[j];
        j++;
        k++;
    }

    free(leftArr);
    free(rightArr);
}

void mergeSortParallel(int *arr, int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;

        if (right - left > THRESHOLD)
        {
#pragma omp task shared(arr)
            mergeSortParallel(arr, left, mid);
#pragma omp task shared(arr)
            mergeSortParallel(arr, mid + 1, right);
#pragma omp taskwait
        }
        else
        {
            mergeSortParallel(arr, left, mid);
            mergeSortParallel(arr, mid + 1, right);
        }

        mergeParallel(arr, left, mid, right);
    }
}

void merge(int *arr, int left, int mid, int right)
{
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int *leftArr = malloc((sizeof(int)) * n1);
    int *rightArr = malloc((sizeof(int)) * n2);

    for (i = 0; i < n1; i++)
        leftArr[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        rightArr[j] = arr[mid + 1 + j];

    i = 0;
    j = 0;
    k = left;

    while (i < n1 && j < n2)
    {
        if (leftArr[i] <= rightArr[j])
        {
            arr[k] = leftArr[i];
            i++;
        }
        else
        {
            arr[k] = rightArr[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        arr[k] = leftArr[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        arr[k] = rightArr[j];
        j++;
        k++;
    }

    free(leftArr);
    free(rightArr);
}

void mergeSort(int *arr, int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;

        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}

// CODE BY Sanish Shyam

void bubble_sort(int *array, int size)
{
    for (int i = 0; i < size - 1; i++)
    {
        for (int j = 0; j < size - i - 1; j++)
        {
            if (array[j] > array[j + 1])
            {
                int temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
}

void parallel_bubble_sort(int *x, int size)
{
    int i, tmp, changes;

    changes = 1; // Set initial condition for the loop
    while (changes)
    {
        changes = 0; // Reset changes flag each iteration

#pragma omp parallel private(tmp)
        {
// Even-indexed comparisons
#pragma omp for reduction(+ : changes)
            for (i = 0; i < size - 1; i += 2)
            {
                if (x[i] > x[i + 1])
                {
                    tmp = x[i];
                    x[i] = x[i + 1];
                    x[i + 1] = tmp;
                    changes++;
                }
            }

// Odd-indexed comparisons
#pragma omp for reduction(+ : changes)
            for (i = 1; i < size - 1; i += 2)
            {
                if (x[i] > x[i + 1])
                {
                    tmp = x[i];
                    x[i] = x[i + 1];
                    x[i + 1] = tmp;
                    changes++;
                }
            }
        }
    }
}

int main()
{

    double wt1, wt2;

    srand(time(NULL));

    omp_set_num_threads(NUMBER_OF_THREADS);

    int *arr = malloc((sizeof(int)) * INPUT_LENGTH);

    for (int i = 0; i < INPUT_LENGTH; i++)
    {
        arr[i] = rand() % INPUT_LENGTH;
    }

    wt1 = omp_get_wtime();

    // ---MERGESORT SERIAL - Artem Anufriyev

    // mergeSort(arr, 0, INPUT_LENGTH - 1);

    // ---MERGESORT PARALLEL - Artem Anufriyev

    // #pragma omp parallel
    //     {
    // #pragma omp single
    //         mergeSortParallel(arr, 0, INPUT_LENGTH - 1);
    //     }

    //--BUBBLESORT SERIAL - Sanish Shyam

    // bubble_sort(arr, INPUT_LENGTH);

    //--BUBBLESORT PARALLEL - Sanish Shyam

    // parallel_bubble_sort(arr, INPUT_LENGTH);

    wt2 = omp_get_wtime();

    printf("\nTotal threads: %d \n", NUMBER_OF_THREADS);
    printf("Size of array: %d", INPUT_LENGTH);
    printf("\nTotal time: %12.4g sec\n", wt2 - wt1);

    for (int i = 1; i < INPUT_LENGTH; i++)
    {
        if (arr[i] >= arr[i - 1])
        {
            continue;
        }
        else
        {
            printf("The sorting is wrong");
            break;
        }
    }

    free(arr);

    return 0;
}