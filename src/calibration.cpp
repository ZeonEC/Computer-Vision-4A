#include <iostream>
#include <filesystem>
#include <opencv2/calib3d.hpp>

#include "calibration.hpp"

void calibration()
{
	std::string images_path = "../../../calibration_images/";

	// chemin de l'image damier
	std::string filename = images_path + "Damier1.png";

	// charger l'image
	cv::Mat image = cv::imread(filename);

	if (image.empty())
	{
		std::cout << "Erreur : impossible de charger l'image\n";
		return;
	}

	// réduire l'image directement
	cv::resize(image, image, cv::Size(), 0.5, 0.5);

	// afficher l'image
	cv::imshow("Damier", image);
	cv::waitKey(0);

	// taille du damier (coins internes)
	cv::Size patternSize(9, 6);

	// conteneur pour les coins
	std::vector<cv::Point2f> corners;

	// detection du damier
	bool found = cv::findChessboardCorners(image, patternSize, corners);

	if (found)
	{
		std::cout << "Damier detecte\n";
		cv::drawChessboardCorners(image, patternSize, corners, found);
	}
	else
	{
		std::cout << "Damier non detecte\n";
	}

	cv::imshow("Damier", image);
	cv::waitKey(0);

}