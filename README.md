# Overview

Self-study experiments focus on the necessity and ease of exploiting parallelism in performance-critical applications. The primary goal is to demonstrate practical and efficient parallel programming techniques, making it an integral part of everyday programming.

## Key Areas of Emphasis

- Techniques to achieve parallelism with minimal effort.

- Understanding and optimizing code performance for modern hardware.

- Analyzing compiler-generated assembly code, CPU execution, and performance prediction.

- Simplifying relevant hardware concepts for better performance optimization.

## Techniques Covered
- Computer programming, algorithms, data structures, C and C++ programming languages.
- Parallel computing, multi-threading, or GPUs.

# Content
## Correlated Pairs Series (cp)

### General Instructions

Given `m` input vectors, each containing `n` numbers. The task is to calculate the correlation between every pair of input vectors in the following function:

```cpp
void correlate(int ny, int nx, const float* data, float* result)
```

For all i and j with 0 <= j <= i < ny, calculate the correlation coefficient between row i of the input matrix and row j of the input matrix. Store the result in result[i + j * ny].

### Should do

- Implement void correlate(int ny, int nx, const float* data, float* result).
- Calculate the correlation coefficient for each pair of input vectors.
- Store the results in the upper triangle of the result matrix.
- Handle numerical precision as specified.

## Image Segmentation Series (is)

### General Instruction

The objective is to partition a given image into two parts: a monochromatic rectangle and a monochromatic background, minimizing the sum of squared errors. This is done by the following function:

```cpp
Result segment(int ny, int nx, const float* data)
```

Data Layout
Color components are numbered `0 <= c < 3`.
X coordinates are numbered `0 <= x < nx`.
Y coordinates are numbered `0 <= y < ny`.
The value of a color component is stored in data`[c + 3 * x + 3 * nx * y]`.

## Median Filter Series (mf)

Implement a program for performing 2-dimensional median filtering using a rectangular window.

### General Instruction

Implement a program for performing 2-dimensional median filtering using a rectangular window in the following function:

```cpp
void mf(int ny, int nx, int hy, int hx, const float* in, float* out)
```

- The arrays in and out are pre-allocated by the caller.

- The original value of pixel (x, y) is given in in[x + nx * y] and its new value will be stored in out[x + nx * y].

- Compute the median value for each pixel (x, y) within the defined window.

- Store the filtered result in the out array.

# Reference
Following requirements and benchmarks from Aalto University course CS-E4580 Programming Parallel Computers.
