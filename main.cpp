#include <iostream>
#include <opencv2/core/hal/interface.h>
#include <opencv/cv.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv/cv.hpp>
#include <unordered_set>
#include "mat_from_file.h"
#include "boundary_tracing.h"
using namespace cv;
using namespace std;

using namespace std;

int main()
{
	const string filename = "C:/CodeProjects/CLion/MMOZ/BoundaryTracing/test_mat.txt";

	cv::Mat_<uchar> orig = readBinImage(filename);

	Mat_<uchar> res = highlightBoundary(orig);


	std::cout << "orig: \n" << orig << "\n\n";
	std::cout << "res: \n" << res;

	cv::waitKey(0);
	return 0;
}