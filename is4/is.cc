#include <omp.h>
#include <algorithm>
#include <cmath>

typedef unsigned long long data_t;

// Helper function for merging two sorted subarrays
void merge(data_t* data, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Create temporary arrays to hold the two subarrays
    data_t* L = new data_t[n1];
    data_t* R = new data_t[n2];

    // Copy data to temporary arrays L[] and R[]
    for (int i = 0; i < n1; i++)
        L[i] = data[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = data[mid + 1 + j];

    // Merge the temporary arrays back into data[]
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            data[k] = L[i];
            i++;
        } else {
            data[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[], if any
    while (i < n1) {
        data[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[], if any
    while (j < n2) {
        data[k] = R[j];
        j++;
        k++;
    }

    // Free memory allocated for temporary arrays
    delete[] L;
    delete[] R;
}

// Recursive function that divides the array into halves, sorts each half independently, and then merges them back together.
void mergeSort(data_t* data, int left, int right, int depth) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        if (depth > 0) {
            #pragma omp task shared(data) if(right - left >= 1000)
            mergeSort(data, left, mid, depth - 1);

            #pragma omp task
            mergeSort(data, mid + 1, right, depth - 1);

            #pragma omp taskwait
            merge(data, left, mid, right);
        } else {
            std::sort(data + left, data + right + 1);
        }
    }
}

// Main parallel sorting function kicks off the parallel sorting process using OpenMP.
void psort(int n, data_t* data) {
    #pragma omp parallel
    {
        #pragma omp single
        mergeSort(data, 0, n - 1, depth);
    }
}
