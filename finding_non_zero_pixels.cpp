//
// Created by Taras Martynyuk on 3/25/2018.
//
#include <cassert>
#include <iostream>
#include "finding_non_zero_pixels.h"
using namespace std;
using namespace cv;
//region kMooreNeighbours coords init

const array<PixelCoords2D, 8> kMooreNeighbours
{
	PixelCoords2D { 0, -1 },	// left
	PixelCoords2D { -1, -1 }, 	// top left
	PixelCoords2D { -1, 0 }, 	// top
	PixelCoords2D { -1, 1 }, 	// top right
	PixelCoords2D { 0, 1 },		// right
	PixelCoords2D { 1, 1 },		// bottom right
	PixelCoords2D { 1, 0 },		// bottom
	PixelCoords2D { 1 , -1 }, 	// bottom left
};
//endregion
//region helper defs

size_t getNeighbourIndex(const PixelCoords2D& start_pixel_coords, const PixelCoords2D& center_pixel_coords);

void setResultPixelCoords(const PixelCoords2D& stumbled_upon_coords, const PixelCoords2D& retreat_to_coords,
	PixelCoords2D& stumbled_upon_pixel, PixelCoords2D& retreated_to_pixel);

bool markAsResultIfNonZero(const int neighbour_coords_index, const cv::Mat_<uchar>& img,
	const PixelCoords2D& center_pixel_coords, PixelCoords2D& stumbled_upon_pixel, PixelCoords2D& retreated_to_pixel);
//endregion

// move clockwise through the 8 neighbours of center_pixel_coords,
// starting from the start_pixel_coords, when the first 8-neighbour is true/1,
// stop. That pixel is in stumbled_upon_pixel var, and the retreated_to_pixel is the previous 8 neighbour pixel
void findFirstNonZeroNeighbourClockwise(const cv::Mat_<uchar>& img, const PixelCoords2D start_pixel_coords,
	const PixelCoords2D center_pixel_coords, PixelCoords2D& stumbled_upon_pixel,
	PixelCoords2D& retreated_to_pixel)
{
	//region params check

	if(center_pixel_coords.i == 0 && center_pixel_coords.j == 0)
		{ throw invalid_argument("center_pixel_coords cannot be top left pixel"); }

	if(! inBounds(img, center_pixel_coords))
		{ throw invalid_argument("center_pixel_coords out of bounds"); }

	if(! inBounds(img, start_pixel_coords))
		{ throw invalid_argument("start_pixel_coords out of bounds"); }

	if(start_pixel_coords == center_pixel_coords)
		{ throw invalid_argument("start_pixel_coords and center_pixel_coords cannot be equal"); }

	if(getPixel(img, start_pixel_coords) != 0)
		{ throw invalid_argument("pixel at start_pixel_coords must be 0"); }

	if(getPixel(img, center_pixel_coords) != 255)
		{ throw invalid_argument("pixel at center_pixel_coords must be 255"); }
	//endregion

	size_t start_pixel_neighbours_index = getNeighbourIndex(start_pixel_coords, center_pixel_coords);

	// iterate clockwise through neighbours
	for (int i = start_pixel_neighbours_index; i < kMooreNeighbours.size(); ++i)
	{
		if(markAsResultIfNonZero(i, img, center_pixel_coords, stumbled_upon_pixel, retreated_to_pixel))
			{ return; }
	}

	for (int j = 0; j < start_pixel_neighbours_index; ++j)
	{
		if(markAsResultIfNonZero(j, img, center_pixel_coords, stumbled_upon_pixel, retreated_to_pixel))
			{ return; }
	}

	throw invalid_argument("no pixels with value 255 in the neighbourhood of center_pixel_coords");
}

// iterate left-to-right through each row starting from top,
// stop when pixel with a value of 255 is found - the value of stumbled_upon_pixel will be
// set to that pix coords, the value of retreated_to_pixel will be set to the previous pixel in iteration
// throws when the top-left pixel is 255
void findFirstNonZeroPixelGoingFromTopLeft(const Mat_<uchar>& img,
										   PixelCoords2D& stumbled_upon_pixel, PixelCoords2D& retreated_to_pixel)
{
	//region find the first foreground pixel
	for (int i = 0; i < img.rows; ++i)
	{
		for (int j = 0; j < img.cols; ++j)
		{
			assert(img(i, j) == 255 || img(i, j) == 0);
			if(img(i, j) == 255)
			{
				stumbled_upon_pixel.i = i;
				stumbled_upon_pixel.j = j;

				// if no pixel to the left, retreat upwards
				if(j == 0)
				{
					if(i == 0)
					{ throw invalid_argument("the top left pixel is foreground(1) - "
												 "the edge pixels must not be foreground pixels"); }
					retreated_to_pixel.i = i - 1;
					retreated_to_pixel.j = j;
				}
				else
				{
					retreated_to_pixel.i = i;
					retreated_to_pixel.j = j - 1;
				}
				return;
			}
		}
	}

	throw invalid_argument("no pixel with value of 255 in the image");
}

//region helpers

// throws if the start_pixel_coords neighbour dir is not in the global moore_neibhbours
size_t getNeighbourIndex(const PixelCoords2D& start_pixel_coords, const PixelCoords2D& center_pixel_coords)
{
	PixelCoords2D start_pixel_dir = start_pixel_coords - center_pixel_coords;

	const PixelCoords2D* start_pixel_neighbours_pos =
		find(kMooreNeighbours.begin(), kMooreNeighbours.end(), start_pixel_dir);

	if(start_pixel_neighbours_pos == kMooreNeighbours.end())
		{ throw invalid_argument("start_pixel_coords must be in the moore neighbourhood of center_pixel_coords"); }

	size_t start_pixel_neighbours_index = start_pixel_neighbours_pos - kMooreNeighbours.begin();
	assert(start_pixel_neighbours_index >= 0 && start_pixel_neighbours_index < kMooreNeighbours.size());

	return start_pixel_neighbours_index;
}

// neighbour_coords_index is the index of this neighbour's coords in moore_coords global array of dirs
// if neighbour pixel is 255, assign it to stumbled_upon_pixel,
// and assign the previous neighbour pixel to retreated_to_pixel
// throws if the neighbour coords are out of bounds,
bool markAsResultIfNonZero(const int neighbour_coords_index, const cv::Mat_<uchar>& img,
	const PixelCoords2D& center_pixel_coords, PixelCoords2D& stumbled_upon_pixel, PixelCoords2D& retreated_to_pixel)
{
	assert(neighbour_coords_index >= 0 && neighbour_coords_index <= kMooreNeighbours.size());

	PixelCoords2D neighbour = kMooreNeighbours[neighbour_coords_index] + center_pixel_coords;

	if(! inBounds(img, neighbour))
		{ throw invalid_argument("neighbour out of bounds"); }

	if(getPixel(img, neighbour) != 255 && getPixel(img, neighbour) != 0)
		{ throw invalid_argument("all pixels of matrix must be either 0 or 255"); }

	if(getPixel(img, neighbour) == 255)
	{
		size_t prev_neighbour_index = neighbour_coords_index - 1 < 0 ?
			kMooreNeighbours.size() - 1 : static_cast<size_t>(neighbour_coords_index - 1);

		PixelCoords2D prev_neighbour = kMooreNeighbours[prev_neighbour_index] + center_pixel_coords;
		assert(inBounds(img, neighbour));

		setResultPixelCoords(neighbour, prev_neighbour, stumbled_upon_pixel, retreated_to_pixel);
		return true;
	}

	return false;
}

// used to set the output params of the findFirstNonZeroNeighbourClockwise
void setResultPixelCoords(const PixelCoords2D& stumbled_upon_val, const PixelCoords2D& retreat_to_val,
						  PixelCoords2D& stumbled_upon_res, PixelCoords2D& retreated_to_res)
{
	stumbled_upon_res.i = stumbled_upon_val.i;
	stumbled_upon_res.j = stumbled_upon_val.j;

	retreated_to_res.i = retreat_to_val.i;
	retreated_to_res.j = retreat_to_val.j;
}

//endregion

