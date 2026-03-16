#include "stereo_calibration.hpp"

#include "gestion_XML.hpp"
#include "calibration.hpp"

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>
#include <vector>

void stereo_calibration() {

	cv::Mat camera_matrix1, dist_coeffs1, camera_matrix2, dist_coeffs2;
	*
	std::vector<cv::Mat> rvecs1, tvecs1, rvecs2, tvecs2;

	std::vector<double> perViewErrors1, perViewErrors2;

	double RMS1, RMS2;

	cv::Size taille_image, pattern_size;

	float square_size;

	std::vector<std::vector<cv::Point2f>> image_points1, image_points2;

	get_calibration_from_xml("calibration_cam1.xml", camera_matrix1, dist_coeffs1, rvecs1, tvecs1,
		perViewErrors1, RMS1, taille_image, pattern_size, square_size, image_points1);
	get_calibration_from_xml("calibration_cam2.xml", camera_matrix2, dist_coeffs2, rvecs2, tvecs2,
		perViewErrors2, RMS2, taille_image, pattern_size, square_size, image_points2);
	cv::Mat R, T;

	cv::stereoCalibrate(image_points1,
		image_points2,
		taille_image,
		camera_matrix1,
		dist_coeffs1,
		camera_matrix2,
		dist_coeffs2,
		R,
		T);
}