//
// Created by Taras Martynyuk on 3/25/2018.
//
#ifndef BOUNDARYTRACING_BOUNDARY_TRACING_H
#define BOUNDARYTRACING_BOUNDARY_TRACING_H

#include <opencv2/core/hal/interface.h>
#include <opencv2/core/mat.hpp>

cv::Mat_<uchar> highlightBoundary(const cv::Mat_<uchar>& img);

#endif //BOUNDARYTRACING_BOUNDARY_TRACING_H
