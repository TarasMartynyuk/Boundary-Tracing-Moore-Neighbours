//
// Created by Taras Martynyuk on 3/24/2018.
//
#include "PixelCoords2D.h"
#include <iostream>
#include <cassert>

const PixelCoords2D operator+(const PixelCoords2D& left, const PixelCoords2D& right)
{
	return PixelCoords2D{ left.i + right.i, left.j + right.j };
}

const PixelCoords2D operator-(const PixelCoords2D& left, const PixelCoords2D& right)
{
	return PixelCoords2D { left.i - right.i, left.j - right.j };
}

bool operator<(const PixelCoords2D&, const PixelCoords2D&)
{
	return true;
}

bool operator==(const PixelCoords2D& left, const PixelCoords2D& right)
{
	return left.i == right.i &&
		   left.j == right.j;
}

bool operator!=(const PixelCoords2D& left, const PixelCoords2D& right)
{
	return ! (left == right);
}


std::ostream& operator<<(std::ostream& os, const PixelCoords2D& coords)
{
	os << '(' << coords.i << ", " << coords.j << ")";
	return os;
}

bool inBounds( const cv::Mat_<uchar>& img, const PixelCoords2D& coords)
{
	return coords.i >= 0 && coords.j >= 0 &&
		coords.i < img.rows && coords.j < img.cols;
}

const uchar& getPixel(const cv::Mat_<uchar>& img, const PixelCoords2D& coords)
{
	assert(inBounds(img, coords));
	return img(coords.i, coords.j);
}

void setPixel(cv::Mat_<uchar>& img, const PixelCoords2D& coords, const uchar new_val)
{
	assert(inBounds(img, coords));
	img(coords.i, coords.j) = new_val;
}

