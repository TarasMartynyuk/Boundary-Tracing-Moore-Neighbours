//
// Created by Taras Martynyuk on 3/25/2018.
//
#ifndef BOUNDARYTRACING_FINDING_NON_ZERO_PIXELS_H
#define BOUNDARYTRACING_FINDING_NON_ZERO_PIXELS_H

#include <opencv2/core/hal/interface.h>
#include <opencv2/core/mat.hpp>
#include "PixelCoords2D.h"

void findFirstNonZeroPixelGoingFromTopLeft(const cv::Mat_<uchar>& img,
	PixelCoords2D& stumbled_upon_pixel, PixelCoords2D& retreated_to_pixel);

void findFirstNonZeroNeighbourClockwise(const cv::Mat_<uchar>& img, const PixelCoords2D start_pixel_coords,
	const PixelCoords2D center_pixel_coords, PixelCoords2D& stumbled_upon_pixel,
	PixelCoords2D& retreated_to_pixel);

#endif //BOUNDARYTRACING_FINDING_NON_ZERO_PIXELS_H
