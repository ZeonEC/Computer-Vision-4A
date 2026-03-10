#ifndef CALIBRATION_HPP
#define CALIBRATION_HPP

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <opencv2/calib3d.hpp>

std::vector<std::vector<cv::Point2f>> get_grid_points(int nb_calib);

#endif 
