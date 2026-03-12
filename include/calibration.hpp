#pragma once

#include <vector>
#include <opencv2/opencv.hpp>

std::vector<std::vector<cv::Point2f>> get_grid_points(int nb_calib);

void show_grid_points(std::vector<std::vector<cv::Point2f>>& images_grid_points);

void calibration(
    std::vector<std::vector<cv::Point2f>>& calibration_points,
    cv::Size& taille_image
);