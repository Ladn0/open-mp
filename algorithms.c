#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

#define INPUT_LENGTH 10000000
#define THRESHOLD 2000
#define THRESHOLD_MERGESORT 1000
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

        if (right - left > THRESHOLD_MERGESORT)
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

// CODE BY Azamat Afzalov

void q_sort(int *arr, int start_index, int end_index)
{
    if (start_index >= end_index)
    {
        return; // Base case: array has one or zero elements
    }

    int pivot = end_index;

    int replace_index = start_index;

    for (int i = start_index; i < end_index; i++)
    {
        if (arr[i] < arr[pivot])
        {
            int tmp = arr[i];
            arr[i] = arr[replace_index];
            arr[replace_index] = tmp;
            replace_index++;
        }
    }

    int tmp = arr[pivot];
    arr[pivot] = arr[replace_index];
    arr[replace_index] = tmp;

    q_sort(arr, start_index, replace_index - 1);
    q_sort(arr, replace_index + 1, end_index);
}

void q_sort_parallel(int *arr, int start_index, int end_index)
{
    if (start_index >= end_index)
    {
        return; // Base case: array has one or zero elements
    }

    int pivot = end_index;

    int replace_index = start_index;

    for (int i = start_index; i < end_index; i++)
    {
        if (arr[i] < arr[pivot])
        {
            int tmp = arr[i];
            arr[i] = arr[replace_index];
            arr[replace_index] = tmp;
            replace_index++;
        }
    }

    int tmp = arr[pivot];
    arr[pivot] = arr[replace_index];
    arr[replace_index] = tmp;

    if (end_index - start_index > THRESHOLD)
    {
#pragma omp task
        q_sort_parallel(arr, start_index, replace_index - 1);

#pragma omp task
        q_sort_parallel(arr, replace_index + 1, end_index);

        // #pragma omp taskwait
    }
    else
    {
        q_sort_parallel(arr, start_index, replace_index - 1);
        q_sort_parallel(arr, replace_index + 1, end_index);
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

    int last_index = INPUT_LENGTH - 1;

    //--QUICKSORT SERIAL - Azamat Afzalov

    // q_sort(arr, 0, last_index);

    //--QUICKSORT PARALLEL - Azamat Afzalov

#pragma omp parallel
    {
#pragma omp single
        {
            q_sort_parallel(arr, 0, last_index);
        }
    }

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