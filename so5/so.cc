#include <algorithm>
#include <vector>
#include <omp.h>
#include <iostream>
#include <iterator>
using namespace std;

typedef unsigned long long data_t;

// Function to find the median of nine elements
data_t medianOfNine(data_t* first, data_t* last) {
    int n = last - first + 1;
    vector<data_t*> samples;

    samples.push_back(first);
    samples.push_back(first + n / 8);
    samples.push_back(first + n / 4);
    samples.push_back(first + 3 * n / 8);
    samples.push_back(first + n / 2);
    samples.push_back(first + 5 * n / 8);
    samples.push_back(first + 3 * n / 4);
    samples.push_back(first + 7 * n / 8);
    samples.push_back(last);

    // Find the median of the nine selected elements
    sort(samples.begin(), samples.end(), [](data_t* a, data_t* b) { return *a < *b; });
    return *samples[4]; // Median of the nine elements
}

// Improved partition function using Hoare's scheme
data_t* hoarePartition(data_t* low, data_t* high, data_t pivot) {
    data_t* i = low - 1;
    data_t* j = high + 1;

    while (true) {
         // Find element on the left side that is greater than or equal to the pivot
        do {
            i++;
        } while (*i < pivot);
        // Find element on the right side that is less than or equal to the pivot
        do {
            j--;
        } while (*j > pivot);
        // If indices have crossed, partitioning is done
        if (i >= j) {
            return j;
        }
        std::swap(*i, *j);
    }
}

// Improved quicksort function using median-of-nine and Hoare partitioning
void quicksort(data_t* first, data_t* last, int threshold) {
    if ((last - first) <= threshold) {
        sort(first, last + 1);
        return;
    }

    data_t pivot = medianOfNine(first, last);
    data_t* pivot_pos = hoarePartition(first, last, pivot);

    #pragma omp task shared(first, pivot_pos)
    quicksort(first, pivot_pos, threshold);
    #pragma omp task shared(pivot_pos, last)
    quicksort(pivot_pos + 1, last, threshold);
}

// Parallel sort function
void psort(int n, data_t* data) {
    int max_thread = omp_get_max_threads();
    #pragma omp parallel
    {
        // to ensure that the size of each subarray handled by a thread is large enough to justify parallel execution
        int threshold = (n / (max_thread * max_thread)) < max_thread ? max_thread : (n / (max_thread * max_thread));
        #pragma omp single
        {
            if (n > 0) quicksort(data, data + n - 1, threshold);
        }
    }
}
