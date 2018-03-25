//
// Created by Taras Martynyuk on 3/25/2018.
//
#include <iostream>
#include <opencv2/core/hal/interface.h>
#include <opencv/cv.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv/cv.hpp>
#include <unordered_set>
#include "PixelCoords2D.h"
#include "finding_non_zero_pixels.h"
using namespace cv;
using namespace std;

//region defs

unordered_set<PixelCoords2D> boundary(const cv::Mat_<uchar>& img);
//endregion

cv::Mat_<uchar> highlightBoundary(const cv::Mat_<uchar>& img)
{
	Mat raw_res(img.rows, img.cols, CV_8UC1, Scalar(0));
	Mat_<uchar> res(raw_res);

	unordered_set<PixelCoords2D> boundary_coords = boundary(img);
	for(const PixelCoords2D& coord : boundary_coords)
	{
		setPixel(res, coord, 255);
	}

	return res;
}

unordered_set<PixelCoords2D> boundary(const cv::Mat_<uchar>& img)
{
	PixelCoords2D curr_stumbled_upon_pix { -1, -1 };
	PixelCoords2D curr_retreated_to_pix { -1, -1 };

	findFirstNonZeroPixelGoingFromTopLeft(img, curr_stumbled_upon_pix, curr_retreated_to_pix);

	const PixelCoords2D first_boundary_pix = curr_stumbled_upon_pix;

	unordered_set<PixelCoords2D> boundary;

	do
	{
		boundary.insert(curr_stumbled_upon_pix);
		PixelCoords2D new_retreated_to_pix;
		PixelCoords2D new_stumbled_upon_pix;
		findFirstNonZeroNeighbourClockwise(img, curr_retreated_to_pix, curr_stumbled_upon_pix,
										   new_stumbled_upon_pix, new_retreated_to_pix);

		assert(inBounds(img, new_retreated_to_pix));
		assert(inBounds(img, new_stumbled_upon_pix));

		curr_stumbled_upon_pix = new_stumbled_upon_pix;
		curr_retreated_to_pix = new_retreated_to_pix;
	}
	while(curr_stumbled_upon_pix != first_boundary_pix);


//	cout << "stumbled_upon_pixel" << curr_stumbled_upon_pix << "\n";
//	cout << "retreated_to_pixel" << curr_retreated_to_pix << "\n";

	return boundary;
}




