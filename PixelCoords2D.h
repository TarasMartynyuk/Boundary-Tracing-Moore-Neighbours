//
// Created by Taras Martynyuk on 3/24/2018.
//
#ifndef REGIONGROWING_PIXEL_H
#define REGIONGROWING_PIXEL_H

#include <ctime>
#include <functional>
#include <opencv2/core/hal/interface.h>
#include <opencv2/core/mat.hpp>

struct PixelCoords2D
{
	int i;
	int j;
};

const PixelCoords2D operator+(const PixelCoords2D&, const PixelCoords2D&);
const PixelCoords2D operator-(const PixelCoords2D&, const PixelCoords2D&);

bool operator==(const PixelCoords2D&, const PixelCoords2D&);
bool operator!=(const PixelCoords2D&, const PixelCoords2D&);
bool operator<(const PixelCoords2D&, const PixelCoords2D&);

std::ostream& operator<<(std::ostream& os, const PixelCoords2D& obj);

bool inBounds( const cv::Mat_<uchar>& im, const PixelCoords2D& coords);
const uchar& getPixel(const cv::Mat_<uchar>&, const PixelCoords2D&);
void setPixel(cv::Mat_<uchar>&, const PixelCoords2D&, const uchar new_val);

namespace std
{
	template <>
	struct hash<PixelCoords2D>
	{
		size_t operator()(const PixelCoords2D & pix) const
		{
			return hash<int>{}(pix.i) + hash<int>{}(pix.j);
		}
	};
}

#endif //REGIONGROWING_PIXEL_H
