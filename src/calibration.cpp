#include <iostream>
#include <filesystem>

#include "calibration.hpp"

void calibration()
{
	std::string images_path = "../../../calibration_images/";

	// chemin de l'image damier
	std::string filename = images_path + "Damier1.jpg";

	// charger l'image
	cv::Mat image = cv::imread(filename);

	if (image.empty())
	{
		std::cout << "Erreur : impossible de charger l'image\n";
		return;
	}

	std::cout << "Image chargee correctement\n";

	// afficher l'image
	cv::imshow("Damier", image);
	cv::waitKey(0);

}