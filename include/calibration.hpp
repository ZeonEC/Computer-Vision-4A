#ifndef CALIBRATION_HPP
#define CALIBRATION_HPP

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <opencv2/calib3d.hpp>

std::vector<std::vector<cv::Point2f>> get_grid_points(int nb_calib);

void calibration(std::vector<std::vector<cv::Point2f>> &calibration_points, cv::Size& taille_image);

void show_grid_points(std::vector<std::vector<cv::Point2f>> &images_grid_points);

void show_images_compare(int nb_to_try, cv::Mat camera_matrix, cv::Mat dist_coeffs, cv::Size& taille_image);

void save_calibration_xml(const std::string& filename,
	const cv::Mat& camera_matrix,
	const cv::Mat& dist_coeffs,
	const std::vector<cv::Mat>& rvecs,
	const std::vector<cv::Mat>& tvecs,
	const std::vector<double>& perViewErrors,
	double RMS,
	const cv::Size& taille_image);

void get_calibration_from_xml(const std::string& filename,
	cv::Mat& camera_matrix,
	cv::Mat& dist_coeffs,
	std::vector<cv::Mat>& rvecs,
	std::vector<cv::Mat>& tvecs,
	std::vector<double>& perViewErrors,
	double& RMS,
	cv::Size& taille_image);
#endif 
