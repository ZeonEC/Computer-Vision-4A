#include <iostream>

// Libs OpenCV
//#include <opencv2/opencv.hpp>
//#include <opencv2/calib3d.hpp>
//#include <opencv2/core.hpp>
//#include <opencv2/highgui.hpp>
//#include <opencv2/improc.hpp>

#include "capture_cam.hpp"
#include "calibration.hpp"

int main() {
	std::cout << "Hello, Computer Vision!" << std::endl;
	std::cout << "BRANCH EC_Calibration" << std::endl;

	// Capture de la camera
	cv::Size taille_image = cv::Size(600, 600);
	int nb_to_try = 20; // capture_cam(taille_image);

	// Recuperation de la grille et affichage dans le terminale
	std::vector<std::vector<cv::Point2f>> calibration_points = get_grid_points(nb_to_try);
	show_grid_points(calibration_points);

	calibration(calibration_points, taille_image);

	return 0;
}
