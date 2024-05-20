#include <cstdlib>
#include <iostream>
#include <cuda_runtime.h>
#include <cmath>

// Utility function to divide and round up
static inline int divup(int a, int b) {
    return (a + b - 1) / b;
}

// Utility function to round up to the nearest multiple of b
static inline int roundup(int a, int b) {
    return divup(a, b) * b;
}

// Utility function to handle CUDA errors
static inline void cudaErrorPrint(cudaError_t err, const char* context) {
    if (err != cudaSuccess) {
        std::cerr << "CUDA error: " << context << " -> Due to "
                  << cudaGetErrorString(err) << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

#define ERRORHANDLER(x) cudaErrorPrint(x, #x)

/**
 * Kernel function for normalizing rows of a matrix.
 *
 * @param numRows Number of rows in the matrix.
 * @param numCols Number of columns in the matrix.
 * @param inputData Pointer to the input data matrix (device memory).
 * @param normalizedData Pointer to the normalized data matrix (device memory).
 */
__global__ void normalizationKernel(int numRows, int numCols, float* inputData, float* normalizedData) {
    int row = blockIdx.x;
    if (row >= numRows) return;

    // Calculate the mean of the row
    float sum = 0.0f;
    for (int col = 0; col < numCols; ++col) {
        sum += inputData[col + row * numCols];
    }
    float mean = sum / static_cast<float>(numCols);

    // Calculate the standard deviation of the row
    float sumSquaredDiffs = 0.0f;
    for (int col = 0; col < numCols; ++col) {
        float value = inputData[col + row * numCols];
        sumSquaredDiffs += (value - mean) * (value - mean);
    }
    float stddev = sqrt(sumSquaredDiffs);

    // Normalize the row
    for (int col = 0; col < numCols; ++col) {
        float value = inputData[col + row * numCols];
        normalizedData[row + col * numRows] = (value - mean) / stddev;
    }
}

/**
 * Kernel function for performing matrix multiplication.
 *
 * @param numRows Number of rows in the matrices.
 * @param numCols Number of columns in the input matrices.
 * @param normalizedData Pointer to the normalized data matrix (device memory).
 * @param result Pointer to the result matrix (device memory).
 */
__global__ void matmulKernel(int numRows, int numCols, float* normalizedData, float* result) {
    int row = threadIdx.x + blockIdx.x * blockDim.x;
    int col = threadIdx.y + blockIdx.y * blockDim.y;
    if (row >= numRows || col >= numRows) return;

    float sum = 0.0f;
    if (row <= col) {
        for (int k = 0; k < numCols; ++k) {
            float x = normalizedData[numRows * k + col];
            float y = normalizedData[numRows * k + row];
            sum += x * y;
        }
    }
    result[col + row * numRows] = sum;
}

/**
 * Function to correlate rows of a matrix using CUDA.
 *
 * @param numRows Number of rows in the input matrix.
 * @param numCols Number of columns in the input matrix.
 * @param data Pointer to the input matrix (host memory).
 * @param result Pointer to the result matrix (host memory).
 */
void correlate(int numRows, int numCols, const float* data, float* result) {
    int dataSize = numRows * numCols * sizeof(float);
    int resultSize = numRows * numRows * sizeof(float);

    float* dInputData = nullptr;
    ERRORHANDLER(cudaMalloc((void**)&dInputData, dataSize));

    float* dNormalizedData = nullptr;
    ERRORHANDLER(cudaMalloc((void**)&dNormalizedData, dataSize));

    float* dResult = nullptr;
    ERRORHANDLER(cudaMalloc((void**)&dResult, resultSize));

    ERRORHANDLER(cudaMemcpy(dInputData, data, dataSize, cudaMemcpyHostToDevice));

    int nBlocks = roundup(numRows, 64);
    normalizationKernel<<<nBlocks, 1>>>(numRows, numCols, dInputData, dNormalizedData);
    ERRORHANDLER(cudaDeviceSynchronize());
    ERRORHANDLER(cudaGetLastError());

    dim3 dimBlock(16, 16);
    dim3 dimGrid(divup(numRows, dimBlock.x), divup(numRows, dimBlock.y));
    matmulKernel<<<dimGrid, dimBlock>>>(numRows, numCols, dNormalizedData, dResult);
    ERRORHANDLER(cudaGetLastError());

    ERRORHANDLER(cudaMemcpy(result, dResult, resultSize, cudaMemcpyDeviceToHost));
    ERRORHANDLER(cudaFree(dInputData));
    ERRORHANDLER(cudaFree(dNormalizedData));
    ERRORHANDLER(cudaFree(dResult));
}
