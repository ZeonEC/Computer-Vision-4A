#ifndef DISPARITY_HPP
#define DISPARITY_HPP

#include <opencv2/opencv.hpp>

void show_disparity(
    const cv::Mat& img_left,
    const cv::Mat& img_right);

#endif