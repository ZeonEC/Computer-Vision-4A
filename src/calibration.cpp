#include <iostream>
#include <filesystem>

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

}