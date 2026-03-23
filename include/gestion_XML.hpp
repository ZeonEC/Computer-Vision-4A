#pragma once

#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

void save_calibration_xml(const std::string& filename,
    const cv::Mat& camera_matrix,
    const cv::Mat& dist_coeffs,
    const std::vector<cv::Mat>& rvecs,
    const std::vector<cv::Mat>& tvecs,
    const std::vector<double>& perViewErrors,
    double RMS,
    const cv::Size& taille_image,
    const cv::Size& pattern_size,
    float square_size,
    const std::vector<std::vector<cv::Point2f>>& image_points);

void get_calibration_from_xml(const std::string& filename,
    cv::Mat& camera_matrix,
    cv::Mat& dist_coeffs,
    std::vector<cv::Mat>& rvecs,
    std::vector<cv::Mat>& tvecs,
    std::vector<double>& perViewErrors,
    double& RMS,
    cv::Size& taille_image,
    cv::Size& pattern_size,
    float& square_size,
    std::vector<std::vector<cv::Point2f>>& image_points);

void save_stereo_calibration_xml(const std::string& filename,
    const cv::Mat& camera_matrix1,
    const cv::Mat& dist_coeffs1,
    const cv::Mat& camera_matrix2,
    const cv::Mat& dist_coeffs2,
    const cv::Mat& R,
    const cv::Mat& T,
    const cv::Mat& E,
    const cv::Mat& F);

void get_stereo_calibration_from_xml(const std::string& filename,
    cv::Mat& camera_matrix1,
    cv::Mat& dist_coeffs1,
    cv::Mat& camera_matrix2,
    cv::Mat& dist_coeffs2,
    cv::Mat& R,
    cv::Mat& T,
    cv::Mat& E,
    cv::Mat& F);