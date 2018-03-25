//
// Created by Taras Martynyuk on 3/24/2018.
//
#include "mat_from_file.h"
#include <fstream>
#include <cassert>
#include <unordered_set>

using namespace std;
using namespace cv;

//region defs

cv::Mat_<bool> readImageCheckPixelVals(const std::string& filename, const std::unordered_set<uchar>* const allowed_vals);
//endregion

cv::Mat_<uchar> readGrayscaleImage(const std::string& filename)
{
	return readImageCheckPixelVals(filename, nullptr);
}

cv::Mat_<bool> readBinImage(const std::string& filename)
{
	unordered_set<uchar> allowed_values;
	allowed_values.insert(0);
	allowed_values.insert(255);

	return readImageCheckPixelVals(filename, &allowed_values);
}

cv::Mat_<bool> readImageCheckPixelVals(const std::string& filename, const std::unordered_set<uchar>* const allowed_vals)
{
	std::ifstream file(filename);
	//region rows cols parse

	int rows, cols;
	try
	{
		file >> rows;
		file >> cols;
	}
	catch (const ios_base::failure&)
	{ throw ios_base::failure("error parsing rows and cols of mat"); }

	if(rows <= 0 && cols <= 0)
	{ throw invalid_argument("rows and cols must be > 0"); }
	//endregion

	Mat raw_mat(rows, cols, CV_8UC1, Scalar(0));
	Mat_<uchar> mat(raw_mat);

	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < cols; ++j)
		{
			try
			{
				int pixel_val;
				file >> pixel_val;

				uchar pixel_val_uchar = (uchar) pixel_val;

				if(pixel_val > 255 || pixel_val < 0)
					{ throw invalid_argument("pixel values must be from 0 to 255"); }

				if(allowed_vals != nullptr && allowed_vals->count(pixel_val_uchar) == 0)
				{
					throw invalid_argument("the pixel value {" + std::to_string(pixel_val_uchar) +
						"} is not in the set of allowed values"); //TODO: pass allowed values as string
				}
				mat(i, j) = pixel_val_uchar;
			}
			catch (const ios_base::failure&)
			{ throw  ios_base::failure("can't parse pixel value"); }
		}
	}
	return mat;
}