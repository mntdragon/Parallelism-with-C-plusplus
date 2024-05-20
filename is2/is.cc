#include <vector>
#include <cmath>
#include <limits>

// Constants for error codes
const int ERROR_CODE = -1;

struct Result
{
    int y0, x0, y1, x1;
    float outer[3], inner[3];
};

void computeIntegralImages(int imageHeight, int imageWidth, const float *imageData,
                           std::vector<std::vector<double>> &sumRed,
                           std::vector<std::vector<double>> &sumGreen,
                           std::vector<std::vector<double>> &sumBlue,
                           std::vector<std::vector<double>> &sumRedSq,
                           std::vector<std::vector<double>> &sumGreenSq,
                           std::vector<std::vector<double>> &sumBlueSq)
{
    // Compute integral images for sums and squared sums of each color channel
    for (int y = 1; y <= imageHeight; ++y)
    {
        for (int x = 1; x <= imageWidth; ++x)
        {
            int idx = 3 * (x - 1 + imageWidth * (y - 1));
            double red = imageData[idx];
            double green = imageData[idx + 1];
            double blue = imageData[idx + 2];

            sumRed[y][x] = red + sumRed[y][x - 1] + sumRed[y - 1][x] - sumRed[y - 1][x - 1];
            sumGreen[y][x] = green + sumGreen[y][x - 1] + sumGreen[y - 1][x] - sumGreen[y - 1][x - 1];
            sumBlue[y][x] = blue + sumBlue[y][x - 1] + sumBlue[y - 1][x] - sumBlue[y - 1][x - 1];

            sumRedSq[y][x] = red * red + sumRedSq[y][x - 1] + sumRedSq[y - 1][x] - sumRedSq[y - 1][x - 1];
            sumGreenSq[y][x] = green * green + sumGreenSq[y][x - 1] + sumGreenSq[y - 1][x] - sumGreenSq[y - 1][x - 1];
            sumBlueSq[y][x] = blue * blue + sumBlueSq[y][x - 1] + sumBlueSq[y - 1][x] - sumBlueSq[y - 1][x - 1];
        }
    }
}

// Function to calculate error values for a given segmentation
double calculateError(int imageHeight, int imageWidth,
                      const std::vector<std::vector<double>> &sumRed,
                      const std::vector<std::vector<double>> &sumGreen,
                      const std::vector<std::vector<double>> &sumBlue,
                      const std::vector<std::vector<double>> &sumRedSq,
                      const std::vector<std::vector<double>> &sumGreenSq,
                      const std::vector<std::vector<double>> &sumBlueSq,
                      int top, int left, int bottom, int right,
                      float &meanRedOuter, float &meanGreenOuter, float &meanBlueOuter,
                      float &meanRedInner, float &meanGreenInner, float &meanBlueInner)
{
    // Calculate the area and sum of squared sums for the inner region
    double areaInner = (bottom - top) * (right - left);
    double sumRedInner = sumRed[bottom][right] - sumRed[bottom][left] - sumRed[top][right] + sumRed[top][left];
    double sumGreenInner = sumGreen[bottom][right] - sumGreen[bottom][left] - sumGreen[top][right] + sumGreen[top][left];
    double sumBlueInner = sumBlue[bottom][right] - sumBlue[bottom][left] - sumBlue[top][right] + sumBlue[top][left];

    // Calculate the mean color values for the inner region
    meanRedInner = sumRedInner / areaInner;
    meanGreenInner = sumGreenInner / areaInner;
    meanBlueInner = sumBlueInner / areaInner;

    double sumRedSqInner = sumRedSq[bottom][right] - sumRedSq[bottom][left] - sumRedSq[top][right] + sumRedSq[top][left];
    double sumGreenSqInner = sumGreenSq[bottom][right] - sumGreenSq[bottom][left] - sumGreenSq[top][right] + sumGreenSq[top][left];
    double sumBlueSqInner = sumBlueSq[bottom][right] - sumBlueSq[bottom][left] - sumBlueSq[top][right] + sumBlueSq[top][left];

    // Calculate the sum of squared sums for the outer region
    double sumRedOuter = sumRed[imageHeight][imageWidth] - sumRedInner;
    double sumGreenOuter = sumGreen[imageHeight][imageWidth] - sumGreenInner;
    double sumBlueOuter = sumBlue[imageHeight][imageWidth] - sumBlueInner;
    double areaOuter = imageHeight * imageWidth - areaInner;
    meanRedOuter = areaOuter > 0 ? sumRedOuter / areaOuter : 0;
    meanGreenOuter = areaOuter > 0 ? sumGreenOuter / areaOuter : 0;
    meanBlueOuter = areaOuter > 0 ? sumBlueOuter / areaOuter : 0;

    double sumRedSqOuter = sumRedSq[imageHeight][imageWidth] - sumRedSqInner;
    double sumGreenSqOuter = sumGreenSq[imageHeight][imageWidth] - sumGreenSqInner;
    double sumBlueSqOuter = sumBlueSq[imageHeight][imageWidth] - sumBlueSqInner;

    // Calculate error values
    // inner
    double errorRedInner = sumRedSqInner - 2 * meanRedInner * sumRedInner + areaInner * meanRedInner * meanRedInner;
    double errorGreenInner = sumGreenSqInner - 2 * meanGreenInner * sumGreenInner + areaInner * meanGreenInner * meanGreenInner;
    double errorBlueInner = sumBlueSqInner - 2 * meanBlueInner * sumBlueInner + areaInner * meanBlueInner * meanBlueInner;
    // outer
    double errorRedOuter = sumRedSqOuter - 2 * meanRedOuter * sumRedOuter + areaOuter * meanRedOuter * meanRedOuter;
    double errorGreenOuter = sumGreenSqOuter - 2 * meanGreenOuter * sumGreenOuter + areaOuter * meanGreenOuter * meanGreenOuter;
    double errorBlueOuter = sumBlueSqOuter - 2 * meanBlueOuter * sumBlueOuter + areaOuter * meanBlueOuter * meanBlueOuter;

    // total error
    return errorRedInner + errorGreenInner + errorBlueInner + errorRedOuter + errorGreenOuter + errorBlueOuter;
}

// Function to find the best segmentation
Result findBestSegmentation(int imageHeight, int imageWidth,
                            const std::vector<std::vector<double>> &sumRed,
                            const std::vector<std::vector<double>> &sumGreen,
                            const std::vector<std::vector<double>> &sumBlue,
                            const std::vector<std::vector<double>> &sumRedSq,
                            const std::vector<std::vector<double>> &sumGreenSq,
                            const std::vector<std::vector<double>> &sumBlueSq)
{
    Result bestResult;
    double minError = std::numeric_limits<double>::max();

    // Iterate through all possible rectangles
    for (int top = 0; top < imageHeight; ++top)
    {
        for (int left = 0; left < imageWidth; ++left)
        {
            for (int bottom = top + 1; bottom <= imageHeight; ++bottom)
            {
                for (int right = left + 1; right <= imageWidth; ++right)
                {
                    // Calculate error for the current segmentation
                    float meanRedOuter, meanGreenOuter, meanBlueOuter;
                    float meanRedInner, meanGreenInner, meanBlueInner;
                    double totalError = calculateError(imageHeight, imageWidth, sumRed, sumGreen, sumBlue, sumRedSq, sumGreenSq, sumBlueSq,
                                                       top, left, bottom, right, meanRedOuter, meanGreenOuter, meanBlueOuter,
                                                       meanRedInner, meanGreenInner, meanBlueInner);

                    // Update the best result if the error is lower
                    if (totalError < minError)
                    {
                        minError = totalError;
                        bestResult = {top, left, bottom, right, {meanRedOuter, meanGreenOuter, meanBlueOuter}, {meanRedInner, meanGreenInner, meanBlueInner}};
                    }
                }
            }
        }
    }

    // Return the best segmentation result found
    return bestResult;
}

Result segment(int ny, int nx, const float *data)
{
    // Check for invalid image dimensions and null input data
    if (ny <= 0 || nx <= 0 || data == nullptr)
    {
        return Result{ERROR_CODE, ERROR_CODE, ERROR_CODE, ERROR_CODE, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}};
    }

    // Initialize integral images for each color channel
    std::vector<std::vector<double>> sumRed(ny + 1, std::vector<double>(nx + 1, 0));
    std::vector<std::vector<double>> sumGreen(ny + 1, std::vector<double>(nx + 1, 0));
    std::vector<std::vector<double>> sumBlue(ny + 1, std::vector<double>(nx + 1, 0));
    // Initialize sum of squared sum tables for each color channel
    std::vector<std::vector<double>> sumRedSq(ny + 1, std::vector<double>(nx + 1, 0));
    std::vector<std::vector<double>> sumGreenSq(ny + 1, std::vector<double>(nx + 1, 0));
    std::vector<std::vector<double>> sumBlueSq(ny + 1, std::vector<double>(nx + 1, 0));

    computeIntegralImages(ny, nx, data, sumRed, sumGreen, sumBlue, sumRedSq, sumGreenSq, sumBlueSq);

    return findBestSegmentation(ny, nx, sumRed, sumGreen, sumBlue, sumRedSq, sumGreenSq, sumBlueSq);
}
