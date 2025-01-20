#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

#define INPUT_LENGTH 100000000

void merge(int *arr, int left, int mid, int right)
{
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Create temporary arrays
    int *leftArr = malloc((sizeof(int)) * n1);
    int *rightArr = malloc((sizeof(int)) * n2);

    // Copy data to temporary arrays
    for (i = 0; i < n1; i++)
        leftArr[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        rightArr[j] = arr[mid + 1 + j];

    // Merge the temporary arrays back into arr[left..right]
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

    // Copy the remaining elements of leftArr[], if any
    while (i < n1)
    {
        arr[k] = leftArr[i];
        i++;
        k++;
    }

    // Copy the remaining elements of rightArr[], if any
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

int main()
{

    double wt1, wt2;

    srand(time(NULL));

    int *arr = malloc((sizeof(int)) * INPUT_LENGTH);

    for (int i = 0; i < INPUT_LENGTH; i++)
    {
        arr[i] = rand() % INPUT_LENGTH;
    }

    wt1 = omp_get_wtime();

    mergeSort(arr, 0, INPUT_LENGTH - 1);

    wt2 = omp_get_wtime();

    printf("\nSize of array: %d", INPUT_LENGTH);
    printf("\nTotal time: %12.4g sec\n", wt2 - wt1);

    free(arr);

    return 0;
}
