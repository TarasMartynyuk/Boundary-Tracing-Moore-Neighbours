//
// Created by Taras Martynyuk on 3/24/2018.
//
#ifndef REGIONGROWING_MAT_FROM_FILE_H
#define REGIONGROWING_MAT_FROM_FILE_H

#include <opencv2/core/hal/interface.h>
#include <opencv2/core/mat.hpp>

cv::Mat_<uchar> readGrayscaleImage(const std::string& filename);
cv::Mat_<bool> readBinImage(const std::string& filename);


#endif //REGIONGROWING_MAT_FROM_FILE_H
