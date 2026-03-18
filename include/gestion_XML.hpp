#pragma once

#include <string>
#include <vector>

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