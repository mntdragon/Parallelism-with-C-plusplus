#include <omp.h>
#include <vector>
#include <algorithm> // std::max_element

int partition(std::vector<float> &arr, int low, int high)
{
  float pivot = arr[high];
  int i = low - 1;
  for (int j = low; j <= high - 1; j++)
  {
    if (arr[j] <= pivot)
    {
      i++;
      std::swap(arr[i], arr[j]);
    }
  }
  std::swap(arr[i + 1], arr[high]);
  // returns the index of the pivot element
  return i + 1;
}

// Using quickSelectHelper algorithm - recursice helper function
float quickSelectHelper(std::vector<float> &arr, int low, int high, int k)
{
  if (low <= high)
  {
    int pIndex = partition(arr, low, high);
    if (pIndex == k)
    {
      return arr[pIndex];
    }
    else if (pIndex < k)
    {
      return quickSelectHelper(arr, pIndex + 1, high, k);
    }
    else
    {
      return quickSelectHelper(arr, low, pIndex - 1, k);
    }
  }
  return 0.0; // Default return, should never be reached
}

// Function to calculate the median of a vector
float calculateMedian(std::vector<float> &data)
{
  std::size_t n = data.size();
  if (n == 0)
  {
    return 0.0;
  }
  int mid = n / 2;
  float median = quickSelectHelper(data, 0, n - 1, mid);
  if (n % 2 == 0)
  {
    float median2 = quickSelectHelper(data, 0, n - 1, mid - 1);
    median = (median + median2) / 2.0;
  }
  return median;
}

// Function to get the valid index range for x and y
void getIndexRange(int ny, int nx, int hy, int hx, int y, int x, int &yUp, int &yDown, int &xLeft, int &xRight)
{
  yUp = std::max((y - hy), 0);
  yDown = std::min((y + hy), (ny - 1));
  xLeft = std::max((x - hx), 0);
  xRight = std::min((x + hx), (nx - 1));
}

// Parallelized version of mf function
void mf(int ny, int nx, int hy, int hx, const float *in, float *out)
{
#pragma omp parallel for
  for (int y = 0; y < ny; ++y)
  {
    std::vector<float> values;
    values.reserve(hx * hy);

    for (int x = 0; x < nx; ++x)
    {
      int yUp, yDown, xLeft, xRight;
      getIndexRange(ny, nx, hy, hx, y, x, yUp, yDown, xLeft, xRight);

      values.clear();
      for (int j = yUp; j <= yDown; ++j)
      {
        for (int i = xLeft; i <= xRight; ++i)
        {
          values.push_back(in[i + (j * nx)]);
        }
      }
      out[x + (y * nx)] = calculateMedian(values);
    }
  }
#pragma omp flush
}