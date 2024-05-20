#pragma once

void NewFunction(int ny, int nx, std::vector<std::vector<float>> &sumR, const float *data, std::vector<std::vector<float>> &sumG, std::vector<std::vector<float>> &sumB);

void error_calc(int ny, int nx, const float * data, float meanR_inner, float meanG_inner, float meanB_inner, int x0, int x1, int y0, int y1, float &error, float outerR, float outerG, float outerB);

void error_calculation(int ny, int nx, const float * data, int x0, int x1, int y0, int y1, float meanR_inner, float outerR, float meanG_inner, float outerG, float meanB_inner, float outerB, float &error);
