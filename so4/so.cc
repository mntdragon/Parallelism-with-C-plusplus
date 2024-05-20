#include <algorithm>
#include <iostream>
#include <vector>
#include <omp.h>
#include <cmath>

typedef unsigned long long data_t;

const int MIN_SORT_SIZE = 5;

int nearsestpoweroftwo(int x)
{
    x = x | (x >> 1);
    x = x | (x >> 2);
    x = x | (x >> 4);
    x = x | (x >> 8);
    x = x | (x >> 16);
    return x - (x >> 1);
}

// Performs parallel sorting of an array of data elements using the merge sort algorithm.
// If the input size is smaller than a threshold, it falls back to std::sort.
// Arguments:
//   - n: The size of the input array.
//   - data: A pointer to the input array.
// Behavior:
//   - Splits the data into blocks and sorts them independently in parallel.
//   - Merges the sorted blocks using inplace_merge.
void psort(int n, data_t *data)
{
    if (n <= 0 || data == nullptr)
    {
        std::cerr << "Error: Invalid input parameters. n must be greater than 0 and data must not be nullptr." << std::endl;
        return;
    }

    int numBlocks = nearsestpoweroftwo(omp_get_max_threads()); // Determine the number of threads dynamically
    int blockSize = (n + numBlocks - 1) / numBlocks;

    if (n < MIN_SORT_SIZE)
    {
        std::sort(data, data + n);
    }
    else
    {
        omp_set_dynamic(0);
#pragma omp parallel num_threads(numBlocks)
        {
            int threadId = omp_get_thread_num();
            int span = threadId * blockSize;
            std::sort(data + std::min(span, n), data + std::min(span + blockSize, n));
        }

        numBlocks /= 2;
        while (numBlocks > 0)
        {
#pragma omp parallel num_threads(numBlocks)
            {
                int t = omp_get_thread_num() * 2;
                int span = t * blockSize;
                std::inplace_merge(data + std::min(span, n),
                                   data + std::min(span + blockSize, n),
                                   data + std::min(span + 2 * blockSize, n));
            }
            blockSize *= 2;
            numBlocks /= 2;
        }
    }
}