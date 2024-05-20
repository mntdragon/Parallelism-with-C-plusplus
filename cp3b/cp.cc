#include <cmath>
#include <vector>
#include <x86intrin.h>
#include <numeric>

inline void* aligned_malloc(std::size_t bytes) {
    return std::aligned_alloc(32, bytes);
}

typedef float float8_t __attribute__ ((vector_size (32)));

const float8_t float8_0 = {0, 0, 0, 0, 0, 0, 0, 0}; // Constant vector of zeros

inline float8_t* float8_alloc(std::size_t n) {
    return static_cast<float8_t*>(aligned_malloc(sizeof(float8_t) * n));
}

static inline float8_t swap4(float8_t x) { return _mm256_permute2f128_ps(x, x, 0b00000001); }
static inline float8_t swap2(float8_t x) { return _mm256_permute_ps(x, 0b01001110); }
static inline float8_t swap1(float8_t x) { return _mm256_permute_ps(x, 0b10110001); }

// Calculate the mean of a row in the input data
float calculate_row_mean(const float* row, int nx) {
    float sum = std::accumulate(row, row + nx, 0.0f); // Sum up elements of the row
    return sum / nx; // Calculate and return the mean
}

// Calculate the square root of the sum of squares of the differences between each element and the mean
float calculate_row_root_sq_sum(const float* row, int nx, float mean) {
    float square_sum = 0.0f;
    for(int j = 0; j < nx; j++) {
        float val = static_cast<float>(row[j]);
        square_sum += ((val - mean) * (val - mean)); // Accumulate sum of squared differences
    }
    return std::sqrt(square_sum); // Calculate and return square root of the sum
}

// Step 1: Normalize the input data
void normalize_data(int num_rows, int num_cols, const float* data, std::vector<float>& normalized_data) {
    #pragma omp parallel for schedule(static, 1)
    for (int row_index = 0; row_index < num_rows; row_index++) {
        float row_mean = calculate_row_mean(&data[row_index * num_cols], num_cols); // Calculate row mean
        float row_root_sq_sum = calculate_row_root_sq_sum(&data[row_index * num_cols], num_cols, row_mean); // Calculate square root sum
        // Normalize each element of the row
        for (int col_index = 0; col_index < num_cols; col_index++) {
            normalized_data[col_index + row_index * num_cols] = ((float)data[col_index + row_index * num_cols] - row_mean) / row_root_sq_sum;
        }
    }
}

// Step 2: Convert normalized data to vector form for efficient computation
void convert_to_vectors(int num_rows, int num_cols, const std::vector<float>& normalized_data, float8_t* vector_data) {
    const int num_elements_per_vector = 8; // Number of elements per vector
    int num_vectors_per_row = (num_rows + num_elements_per_vector - 1) / num_elements_per_vector; // Calculate number of vectors per row
    #pragma omp parallel for schedule(static, 1)
    for (int row_vector_index = 0; row_vector_index < num_vectors_per_row; row_vector_index++) {
        // Convert each row into vector form
        for (int col_index = 0; col_index < num_cols; col_index++) {
            for (int element_index = 0; element_index < num_elements_per_vector; element_index++) {
                int row_index = row_vector_index * num_elements_per_vector + element_index;
                // Fill vector with normalized data, padding with zeros if necessary
                vector_data[num_cols * row_vector_index + col_index][element_index] = row_index < num_rows ? normalized_data[num_cols * row_index + col_index] : 0.0;
            }
        }
    }
}

// Step 3: Compute correlations between pairs of blocks
void compute_correlations(int num_rows, int num_cols, float8_t* vector_data, float* result) {
    const int num_elements_per_vector = 8; // Number of elements per vector
    int num_vectors_per_row = (num_rows + num_elements_per_vector - 1) / num_elements_per_vector; // Calculate number of vectors per row
    #pragma omp parallel for schedule(static, 1)
    for (int row_block_index = 0; row_block_index < num_vectors_per_row; row_block_index++) {
        for (int col_block_index = row_block_index; col_block_index < num_vectors_per_row; col_block_index++) {
            // Initialize accumulator vectors for storing intermediate results
            float8_t z000 = float8_0, z001 = float8_0, z010 = float8_0, z011 = float8_0;
            float8_t z100 = float8_0, z101 = float8_0, z110 = float8_0, z111 = float8_0;
            // Iterate over each column vector
            
            for (int col_vector_index = 0; col_vector_index < num_cols; col_vector_index++) {
                const int PREFETCH_DISTANCE = 28;
                // Prefetch vector data to improve cache performance
                __builtin_prefetch(&vector_data[num_cols * row_block_index + col_vector_index + PREFETCH_DISTANCE]);
                __builtin_prefetch(&vector_data[num_cols * col_block_index + col_vector_index + PREFETCH_DISTANCE]);
                // Load vector data
                float8_t a000 = vector_data[num_cols * row_block_index + col_vector_index];
                float8_t b000 = vector_data[num_cols * col_block_index + col_vector_index];
                float8_t a100 = swap4(a000);
                float8_t a010 = swap2(a000);
                float8_t a110 = swap2(a100);
                float8_t b001 = swap1(b000);
                // Perform dot product and accumulate results
                z000 += (a000 * b000);
                z001 += (a000 * b001);
                z010 += (a010 * b000);
                z011 += (a010 * b001);
                z100 += (a100 * b000);
                z101 += (a100 * b001);
                z110 += (a110 * b000);
                z111 += (a110 * b001);
            }
            // Store intermediate results in an array of vectors
            float8_t intermediate_results[8] = {z000, z001, z010, z011, z100, z101, z110, z111};
            // Adjust vectors to ensure correct results
            for (int block_index = 1; block_index < num_elements_per_vector; block_index += 2) {
                intermediate_results[block_index] = swap1(intermediate_results[block_index]);
            }
            // Copy results to the output matrix
            for (int col_element_index = 0; col_element_index < num_elements_per_vector; col_element_index++) {
                for (int row_element_index = 0; row_element_index < num_elements_per_vector; row_element_index++) {
                    int row_index = row_element_index + row_block_index * num_elements_per_vector;
                    int col_index = col_element_index + col_block_index * num_elements_per_vector;
                    // Store result if within valid bounds
                    if (col_index < num_rows && row_index < num_rows && row_index <= col_index) {
                        result[num_rows * row_index + col_index] = intermediate_results[row_element_index ^ col_element_index][col_element_index];
                    }
                }
            }
        }
    }
}

// Correlate function computes correlations between elements of the input matrix
void correlate(int num_rows, int num_cols, const float* data, float* result) {
    std::vector<float> normalized_data(num_rows * num_cols);
    normalize_data(num_rows, num_cols, data, normalized_data);
    float8_t* vector_data = float8_alloc((num_rows + 7) / 8 * num_cols); // Allocate memory for vectorized data
    convert_to_vectors(num_rows, num_cols, normalized_data, vector_data);
    compute_correlations(num_rows, num_cols, vector_data, result);
    free(vector_data); // Free allocated memory
}

