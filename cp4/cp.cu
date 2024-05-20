#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cuda_runtime.h>
#include <thrust/device_vector.h>
#include <thrust/copy.h>
#include <thrust/for_each.h>
#include <thrust/execution_policy.h>
#include <thrust/transform.h>

// CUDA error checking
static inline void cudaErrorPrint(cudaError_t err, const char *context)
{
    if (err != cudaSuccess)
    {
        std::cerr << "CUDA error: " << context << ": "
                  << cudaGetErrorString(err) << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

#define ERRORVERIFIER(x) cudaErrorPrint(x, #x)

// Calculate the smallest integer greater than or equal to the result of a division
static inline int divup(int a, int b)
{
    return (a + b - 1) / b;
}

// Round up a number to the nearest multiple of another number
static inline int roundup(int a, int b)
{
    return divup(a, b) * b;
}

// Device function to compute squared difference
__device__ float squared_diff(float x, float mean)
{
    return (x - mean) * (x - mean);
}

// Device function to normalize data
__device__ float normalize(float x, float mean, float resultMatrix)
{
    return (x - mean) / resultMatrix;
}

// Normalize the matrix to ensure that the correlation values are between -1 and 1
__global__ void normalizeMatrixKernel(int ny, int nx, float *data, float *ntdata)
{
    int y = blockIdx.x;
    if (y >= ny)
        return;

    float sum = thrust::transform_reduce(thrust::seq, data + y * nx, data + (y + 1) * nx, thrust::identity<float>(), 0.0f, thrust::plus<float>());
    float mean = sum / static_cast<float>(nx);

    float rs = thrust::transform_reduce(thrust::seq, data + y * nx, data + (y + 1) * nx, thrust::identity<float>(), 0.0f, thrust::plus<float>(), [=] __device__(float x) { return squared_diff(x, mean); });
    float resultMatrix = sqrt(rs);

    for (int x = 0; x < nx; ++x)
    {
        ntdata[y + x * ny] = normalize(data[x + y * nx], mean, resultMatrix);
    }
}

// Perform the matrix multiplication to calculate the correlation matrix
__global__ void multiplyMatricesKernel(int ny, int nx, float *ntdata, float *resultMatrix)
{
    int i = threadIdx.x + blockIdx.x * blockDim.x;
    int j = threadIdx.y + blockIdx.y * blockDim.y;
    if (i >= ny || j >= ny)
        return;

    float sum = thrust::inner_product(thrust::seq, ntdata + j * nx, ntdata + (j + 1) * nx, ntdata + i * nx, 0.0f, thrust::plus<float>(), thrust::multiplies<float>());
    resultMatrix[j + i * ny] = sum;
}

void correlate(int ny, int nx, const float *data, float *result)
{
    const int THREAD_BLOCK_SIZE = 16;
    int rn = ny * ny;

    // Allocate device vectors using thrust
    thrust::device_vector<float> deviceInputData(data, data + ny * nx);
    thrust::device_vector<float> deviceNormalizedData(ny * nx);
    thrust::device_vector<float> deviceResultMatrix(rn);

    cudaStream_t stream1, stream2;
    ERRORVERIFIER(cudaStreamCreate(&stream1));
    ERRORVERIFIER(cudaStreamCreate(&stream2));

    // Normalize data before performing normalization operations
    normalizeMatrixKernel<<<ny, 1, 0, stream1>>>(ny, nx, thrust::raw_pointer_cast(deviceInputData.data()), thrust::raw_pointer_cast(deviceNormalizedData.data()));

    // Multiply matrices
    dim3 threadBlock(THREAD_BLOCK_SIZE, THREAD_BLOCK_SIZE);
    dim3 gridDim(divup(ny, threadBlock.x), divup(ny, threadBlock.y));
    multiplyMatricesKernel<<<gridDim, threadBlock, 0, stream2>>>(ny, nx, thrust::raw_pointer_cast(deviceNormalizedData.data()), thrust::raw_pointer_cast(deviceResultMatrix.data()));

    // Copy result back to host
    thrust::copy(deviceResultMatrix.begin(), deviceResultMatrix.end(), result);

    ERRORVERIFIER(cudaStreamSynchronize(stream2));
    ERRORVERIFIER(cudaStreamDestroy(stream1));
    ERRORVERIFIER(cudaStreamDestroy(stream2));
}
