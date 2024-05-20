#include <vector>
#include <cmath>

using namespace std;

// defines a new type double4_t as a vector of 4 double values
typedef double double4_t __attribute__((vector_size(4 * sizeof(double))));

void calculate_correlations_within_blocks(int numBlocks, double4_t &correlationVector, std::vector<double4_t> &normalizedData, int rowIndex, int paddedBlockSize, int colIndex)
{
    // iterate over each block
    for (int blockIndex = 0; blockIndex < numBlocks; blockIndex++)
    {
        correlationVector += normalizedData[rowIndex * paddedBlockSize + blockIndex] * normalizedData[colIndex * paddedBlockSize + blockIndex];
    }
}
void correlation_calculate(int numRows, int numBlocks, std::vector<double4_t> &normalizedData, int paddedBlockSize, int blockRemainder, float *result)
{
    // iterate over each pair of rows
    for (int rowIndex = 0; rowIndex < numRows; rowIndex++)
    {
        for (int colIndex = rowIndex; colIndex < numRows; colIndex++)
        {
            double4_t correlationVector = {0};
            calculate_correlations_within_blocks(numBlocks, correlationVector, normalizedData, rowIndex, paddedBlockSize, colIndex);
            for (int elementIndex = 0; elementIndex < blockRemainder; elementIndex++)
            {
                // // Calculate correlations for leftover elements
                correlationVector[elementIndex] += normalizedData[rowIndex * paddedBlockSize + numBlocks][elementIndex] * normalizedData[colIndex * paddedBlockSize + numBlocks][elementIndex];
            }
            // assign the correlation in the result array
            result[colIndex + rowIndex * numRows] = (float)(correlationVector[0] + correlationVector[1] + correlationVector[2] + correlationVector[3]);
        }
    }
}

void calculate_sum_of_squares_vector(int numBlocks, std::vector<double4_t> &normalizedData, int rowIndex, int paddedBlockSize, double4_t meanVector, double4_t &sumOfSquaresVector)
{
    for (int blockIndex = 0; blockIndex < numBlocks; blockIndex++)
    {
        normalizedData[rowIndex * paddedBlockSize + blockIndex] -= meanVector;
        sumOfSquaresVector += normalizedData[rowIndex * paddedBlockSize + blockIndex] * normalizedData[rowIndex * paddedBlockSize + blockIndex];
    }
}
void normalize_rows(int numBlocks, int vectorSize, std::vector<double4_t> &normalizedData, int paddedBlockSize, int rowIndex, const float *data, int &numCols, int blockRemainder)
{
    // iterate over each block of the row
    for (int blockIndex = 0; blockIndex < numBlocks; blockIndex++)
    {
        // iterate over each element in the block
        for (int elementIndex = 0; elementIndex < vectorSize; elementIndex++)
        {
            int dataIndex = blockIndex * vectorSize + elementIndex;
            normalizedData[paddedBlockSize * rowIndex + blockIndex][elementIndex] = data[rowIndex * numCols + dataIndex];
        }
    }
    // Copy the leftover elements in the last block
    for (int elementIndex = 0; elementIndex < blockRemainder; elementIndex++)
    {
        normalizedData[paddedBlockSize * rowIndex + numBlocks][elementIndex] = data[rowIndex * numCols + numBlocks * vectorSize + elementIndex];
    }
    double4_t sumVector = {0};
    // start to calculate the sum by iterating voer each block of the row
    for (int blockIndex = 0; blockIndex < paddedBlockSize; blockIndex++)
    {
        sumVector += normalizedData[rowIndex * paddedBlockSize + blockIndex];
    }
    double meanValue = (sumVector[0] + sumVector[1] + sumVector[2] + sumVector[3]) / numCols;
    double4_t meanVector = {meanValue, meanValue, meanValue, meanValue};

    // Calculate sum of squares vector
    double4_t sumOfSquaresVector = {0};
    calculate_sum_of_squares_vector(numBlocks, normalizedData, rowIndex, paddedBlockSize, meanVector, sumOfSquaresVector);
    for (int elementIndex = 0; elementIndex < blockRemainder; elementIndex++)
    {
        normalizedData[rowIndex * paddedBlockSize + numBlocks][elementIndex] -= meanValue;
        sumOfSquaresVector[elementIndex] += normalizedData[rowIndex * paddedBlockSize + numBlocks][elementIndex] * normalizedData[rowIndex * paddedBlockSize + numBlocks][elementIndex];
    }
    double normalizationFactor = 1 / sqrt(sumOfSquaresVector[0] + sumOfSquaresVector[1] + sumOfSquaresVector[2] + sumOfSquaresVector[3]);
    double4_t normalizationVector = {normalizationFactor, normalizationFactor, normalizationFactor, normalizationFactor};

    for (int blockIndex = 0; blockIndex < paddedBlockSize; blockIndex++)
    {
        normalizedData[rowIndex * paddedBlockSize + blockIndex] *= normalizationVector;
    }
}

void normalize_data(int numRows, int numBlocks, int vectorSize, std::vector<double4_t> &normalizedData, int paddedBlockSize, const float *data, int &numCols, int blockRemainder)
{
    //  iterates over each row of the data matrix
    for (int rowIndex = 0; rowIndex < numRows; rowIndex++)
    {
        normalize_rows(numBlocks, vectorSize, normalizedData, paddedBlockSize, rowIndex, data, numCols, blockRemainder);
    }
}

void correlate(int numRows, int numCols, const float *data, float *result)
{
    int vectorSize = 4;
    int numBlocks = numCols / vectorSize;
    int blockRemainder = numCols % vectorSize;
    int paddedBlockSize = (numCols + vectorSize - 1) / vectorSize;

    // allocate mem fo the normalized data
    std::vector<double4_t> normalizedData(numRows * paddedBlockSize);

    // Normalize rows of the input data matrix
    normalize_data(numRows, numBlocks, vectorSize, normalizedData, paddedBlockSize, data, numCols, blockRemainder);

    // calculates the correlation
    correlation_calculate(numRows, numBlocks, normalizedData, paddedBlockSize, blockRemainder, result);
}
