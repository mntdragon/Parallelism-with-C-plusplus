#include <vector>
#include <cmath>
#include <omp.h>

/*
* -  `#pragma omp flush` in several places: ensure data consistency
*/
void normalize_row(std::vector<double>& row)
{
    double sumval = 0, sumvalsquare = 0;
    int size = row.size();

    #pragma omp parallel for reduction(+:sumval, sumvalsquare)
    for (int i = 0; i < size; ++i)
    {
        double dataval = row[i];
        #pragma omp atomic
        sumval += dataval;
        #pragma omp atomic
        sumvalsquare += dataval * dataval;
    }

    const double meanval = sumval / size;
    const double variance = sumvalsquare / size - meanval * meanval;
    const double denom = sqrt(size * variance);

    #pragma omp parallel for 
    for (int i = 0; i < size; ++i)
    {
        row[i] = (row[i] - meanval) / denom;
    }
    #pragma omp flush
}

void normalize_rows(std::vector<std::vector<double>>& data)
{
    #pragma omp parallel for
    for (size_t row = 0; row < data.size(); ++row)
    {
        normalize_row(data[row]);
    }
    #pragma omp flush
}

std::vector<std::vector<double>> matrix_product(const std::vector<std::vector<double>>& data)
{
    size_t ny = data.size();
    size_t nx = data[0].size();

    std::vector<std::vector<double>> result(ny, std::vector<double>(ny, 0.0));

    #pragma omp parallel for collapse(2)
    for (size_t res_row = 0; res_row < ny; ++res_row)
    {
        for (size_t res_col = res_row; res_col < ny; ++res_col)
        {
            double dot_product = 0.0;
            for (size_t idx = 0; idx < nx; ++idx)
            {
                dot_product += data[res_row][idx] * data[res_col][idx];
            }
            result[res_row][res_col] = dot_product;
            if (res_row!= res_col) // If not on the diagonal, set the symmetric element
                result[res_col][res_row] = dot_product;
        }
    }
    #pragma omp flush
    return result;
}

void correlate(int ny, int nx, const float *data, float *result)
{
    omp_set_num_threads(omp_get_max_threads());

    std::vector<std::vector<double>> data_to_be_normalized(ny, std::vector<double>(nx));
    
    // the chunk size should be adjusted to adapt the performance, currently = 32
    #pragma omp parallel for schedule(dynamic, 32)
    for (int row = 0; row < ny; ++row) {
        std::vector<double> row_data(data + row * nx, data + (row + 1) * nx);
        data_to_be_normalized[row] = row_data;
        normalize_row(data_to_be_normalized[row]);
    }
    #pragma omp flush

    // Compute the matrix product
    std::vector<std::vector<double>> result_matrix = matrix_product(data_to_be_normalized);

    // the chunk size should be adjusted to adapt the performance, currently = 32
    #pragma omp parallel for schedule(static, 32)
    for (int i = 0; i < ny * ny; ++i)
    {
        result[i] = result_matrix[i / ny][i % ny];
    }
    #pragma omp flush
}