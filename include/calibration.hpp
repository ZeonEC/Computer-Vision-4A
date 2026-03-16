#pragma once

#include <vector>
#include <opencv2/opencv.hpp>

// détecte les coins du damier dans les images de calibration
std::vector<std::vector<cv::Point2f>> get_grid_points(int nb_calib);

// affiche les coins détectés dans le terminal
void show_grid_points(std::vector<std::vector<cv::Point2f>>& images_grid_points);

// lance la calibration de la caméra
void calibration();