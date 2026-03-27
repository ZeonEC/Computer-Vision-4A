#ifndef STEREO_CALIBRATION_HPP
#define STEREO_CALIBRATION_HPP

#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>

#include <iostream>
#include <vector>

void reverse_chessboard_x(std::vector<cv::Point2f>& corners, const cv::Size& pattern_size);
void reverse_chessboard_y(std::vector<cv::Point2f>& corners, const cv::Size& pattern_size);
void normalize_chessboard_corners(std::vector<cv::Point2f>& corners, const cv::Size& pattern_size);

void draw_corner_indices(cv::Mat& image, const std::vector<cv::Point2f>& corners, const cv::Size& pattern_size);

// Affichage interactif + recuperation des points stereo corriges
void get_stereo_grid_points(
    int& nb_calib,
    std::vector<std::vector<cv::Point2f>>& image_points0,
    std::vector<std::vector<cv::Point2f>>& image_points1
);

// Affichage simple de la detection des coins pour les images de calibration stereo
void show_stereo_chessboard_detection(int& nb_calib);

void stereo_calibration(int& nb_calib);

#endif // STEREO_CALIBRATION_HPP