#include <cmath>
#include <vector>
#include <x86intrin.h>
#include <execution>
#include <numeric>
#include <immintrin.h>

// Function to allocate aligned memory
inline void* aligned_malloc(std::size_t bytes) {
   return std::aligned_alloc(32, bytes);
}

// Define a vector data type with 4 elements of type double
typedef double double4_t __attribute__ ((vector_size (4 * sizeof(double))));

// Function to allocate an array of double4_t with alignment
inline double4_t* double4_alloc(std::size_t n) {
    return static_cast<double4_t*>(aligned_malloc(sizeof(double4_t) * n));
}

// Swap the high and low 128-bit halves of a double4_t vector
static inline double4_t swap_high_low(double4_t x) {
    return _mm256_permute2f128_pd(x, x, 0b00000001);
}

// Swap the two adjacent doubles within each 128-bit half of a double4_t vector
static inline double4_t swap_high_low_same(double4_t x) {
    return _mm256_permute_pd(x, 0b00000101);
}

// Calculate the mean of a row in the input data
double calculate_row_mean(const float* row, int nx){
    // Use std::accumulate to sum up the elements of the row
    double sum = std::accumulate(row, row + nx, 0.0);
    // Divide the sum by the number of elements to get the mean
    return sum / static_cast<double>(nx);
}

// Calculate the square root of the sum of squares of the differences between each element and the mean
double calculate_row_root_sq_sum(const float* row, int nx, double mean){
   double square_sum = 0.0;
    // Iterate over each element of the row
    for(int j=0; j < nx; j++){
        double val = static_cast<double>(row[j]);
        // Square the difference between the element and the mean, and accumulate
        square_sum += ((val - mean) * (val - mean));
    }
    // Take the square root of the accumulated sum
    return sqrt(square_sum);
}

// Function to normalize the input data
void normalize_data(int ny, int nx, const float* data, std::vector<double>& normalized_data) {
    // Calculate mean and root square sum for each row in parallel
    #pragma omp parallel for schedule(static, 1)
    for (int j=0; j < ny; j++){
        double row_mean = calculate_row_mean(&data[j * nx], nx);
        double row_root_sq_sum = calculate_row_root_sq_sum(&data[j * nx], nx, row_mean);
        // Normalize each element of the row and store in the normalized data vector
        #pragma omp simd
        for(int i=0; i < nx; i++){
            normalized_data[i + j * nx] = (((double) data[i + j * nx]) - row_mean) / row_root_sq_sum;
        }
    }
}

// Function to convert normalized data to vector form
void convert_to_vector_form(int ny, int nx, const std::vector<double>& normalized_data, double4_t* vector_data) {
    // Determine the number of vectors per block
    const int num_vectors_per_block = 4;
    // Calculate the number of blocks needed to cover all rows
    int num_blocks = (ny + num_vectors_per_block - 1) / num_vectors_per_block;

    // Vectorization: Convert normalized data to vector form
    #pragma omp parallel for schedule(static, 1)
    for(int block_index=0; block_index < num_blocks; block_index++){
         #pragma omp simd
        for(int i=0; i < nx; i++){
            for(int vector_index=0; vector_index < num_vectors_per_block; vector_index++){
                const int PREFETCH_DISTANCE = 28;
                int j = num_vectors_per_block * block_index + vector_index;
                // Prefetch the data to improve cache performance
                __builtin_prefetch(&normalized_data[nx * j + i + PREFETCH_DISTANCE]);
                // Load the data into a vector, padding with zeros if necessary
                vector_data[nx * block_index + i][vector_index] = j < ny ? normalized_data[nx * j + i] : 0.0;
            }
        }
    }
}

// Function to compute correlations between pairs of blocks
void compute_correlations(int ny, int nx, double4_t* vector_data, float* result) {
    // Determine the number of vectors per block
    const int num_vectors_per_block = 4;
    // Calculate the number of blocks needed to cover all rows
    int num_blocks = (ny + num_vectors_per_block - 1) / num_vectors_per_block;

    // Compute correlations between pairs of blocks
    #pragma omp parallel for schedule(dynamic)
    for(int block_row_index=0; block_row_index < num_blocks; block_row_index++){
        for(int block_col_index = block_row_index; block_col_index < num_blocks; block_col_index++){
            double4_t z00 = {0,0,0,0};
            double4_t z01 = {0,0,0,0};
            double4_t z10 = {0,0,0,0};
            double4_t z11 = {0,0,0,0};
             #pragma omp simd
            for(int k=0; k < nx; k++){
                const int PREFETCH_DISTANCE = 20;
                // Prefetch the vectorized data to improve cache performance
                __builtin_prefetch(&vector_data[nx * block_row_index + k + PREFETCH_DISTANCE]);
                __builtin_prefetch(&vector_data[nx * block_col_index + k + PREFETCH_DISTANCE]);

                // Load vectors for the current iteration
                double4_t a00 = vector_data[nx * block_row_index + k];
                double4_t b00 = vector_data[nx * block_col_index + k];
                double4_t a10 = swap_high_low(a00);
                double4_t b01 = swap_high_low_same(b00);

                // Perform dot products and accumulate the results
                z00 = z00 + (a00 * b00);
                z01 = z01 + (a00 * b01);
                z10 = z10 + (a10 * b00);
                z11 = z11 + (a10 * b01);
            }
            // Store the results in an array of vectors
            double4_t z[4] = {z00, z01, z10, z11};
            // Swap high and low parts of vectors to ensure correct results
            for(int kb=1; kb < num_vectors_per_block; kb += 2){
                z[kb] = swap_high_low_same(z[kb]);
            }
            // Copy results to the output matrix
            for(int jb=0; jb < num_vectors_per_block; jb++){
                for(int ib=0; ib < num_vectors_per_block; ib++){
                    int i = ib + num_vectors_per_block * block_row_index;
                    int j = jb + num_vectors_per_block * block_col_index;
                    // Store the result only if it lies within the upper triangular portion of the matrix
                    if(j < ny && i < ny && i <= j){
                        result[ny * i + j] = z[ib ^ jb][jb];
                    }
                }
            }
        }
    }
}

// Main correlation function
void correlate(int ny, int nx, const float* data, float* result) {
    // Allocate memory for storing normalized data
    std::vector<double> normalized_data(ny*nx);

    // Normalize the input data
    normalize_data(ny, nx, data, normalized_data);

    // Allocate memory for storing vectorized data
    double4_t* vector_data = double4_alloc(ny * nx);

    // Convert normalized data to vector form
    convert_to_vector_form(ny, nx, normalized_data, vector_data);

    // Compute correlations between pairs of blocks
    compute_correlations(ny, nx, vector_data, result);

    // Free allocated memory
    free(vector_data);
}
