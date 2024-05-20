
#include <vector>
#include <algorithm>
#include <cmath>

/*
* Function to normalize a row
* To optimize:
* - Break down the loop iterations into smaller loops 
* with unroll_factor_num per loop iteration for better instruction scheduling.
* - Single Instruction, Multiple Data from vectorization, aligning with the loop unrolling instructions.
*/
void normalize_row(double *row, int size)
{
    // adjustable for better match size of data at a time in the loops
    const int unroll_factor_num = 4; 

    double sumval[unroll_factor_num] = {0}, sumvalsquare[unroll_factor_num] = {0};
    double meanval = 0, variance = 0;

    for (int i = 0; i < size; i += unroll_factor_num)
    {
        for (int j = 0; j < unroll_factor_num; ++j)
        {
            if (i + j < size)
            {
                double dataval = row[i + j];
                sumval[j] += dataval;
                sumvalsquare[j] += dataval * dataval;
            }
        }
    }

    // Accumulate the sums
    for (int j = 0; j < unroll_factor_num; ++j)
    {
        meanval += sumval[j];
        variance += sumvalsquare[j];
    }

    meanval /= size;
    variance = variance / size - meanval * meanval;

    for (int i = 0; i < size; i += unroll_factor_num)
    {
        for (int j = 0; j < unroll_factor_num; ++j)
        {
            if (i + j < size)
            {
                row[i + j] = (row[i + j] - meanval) / sqrt(size * variance);
            }
        }
    }
}

// Function to normalize all rows
void normalize_rows(double *data, int ny, int nx)
{
    for (int row = 0; row < ny; ++row)
    {
        normalize_row(data + row * nx, nx);
    }
}
/*
* Function to perform matrix multiplication
* To optimize:
* - Apply same unroll_factor_num techniques to loop over the `idx`
* - ALong with unrolling loop, combine multiple iterations of a loop into a single iteration 
* to execute multiple multiplications
*/
void matrix_product(const double *X, int ny, int nx, double *result)
{
    // adjustable for better match size of data at a time in the loops
    const int unroll_factor_num = 4;

    for (int res_row = 0; res_row < ny; ++res_row)
    {
        for (int res_col = res_row; res_col < ny; ++res_col)
        {
            double dot_product[unroll_factor_num] = {0};

        // Iterate over the columns of the input matrix using unrolled loop
            for (int idx = 0; idx < nx; idx += unroll_factor_num)
            {
                // Iteration for dot product computation
                for (int i = 0; i < unroll_factor_num; ++i)
                {
                    if (idx + i < nx)
                    {
                        dot_product[i] += X[res_row * nx + idx + i] * X[res_col * nx + idx + i];
                    }
                }
            }

        // To sum the partial of dot products
            double sum = 0;
            for (int i = 0; i < unroll_factor_num; ++i)
            {
                sum += dot_product[i];
            }
        // Assigning the result 
            result[res_row * ny + res_col] = sum;
            result[res_col * ny + res_row] = sum; // Symmetric, so set both elements
        }
    }
}

void correlate(int ny, int nx, const float *data, float *result)
{
    std::vector<double> datav(data, data + nx * ny);
    std::vector<double> resultv(ny * ny);

    normalize_rows(datav.data(), ny, nx);

    // compute the pairwise dot products of rows in the input matrix
    matrix_product(datav.data(), ny, nx, resultv.data());

    // Assign the result vector to result array
    for (int i = 0; i < ny * ny; ++i)
    {
        result[i] = resultv[i];
    }
}
