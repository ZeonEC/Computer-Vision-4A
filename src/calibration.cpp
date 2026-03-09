#include "calibration.h"

void calibrate_camera() {

	// Charger une image de calibration
	cv::Mat calib_image = cv::imread("../../../calibration_images/calib1.jpg");

	// verifie si l'image a été chargée correctement
	if (calib_image.empty()) {
		std::cerr << "Erreur: impossible de charger l'image de calibration.\n";
		return;
	}
	else {
		std::cout << "Image de calibration chargée avec succès.\n";
	}

	// on crée une vecteur de points pour la calibration
	std::vector<cv::Point2f> corners;
	cv::Size pattern_size(3, 7); // nombre de coins intérieurs dans la grille (7x9)

	// Trouver les coins de la grille, on laisse le FLAGS par défaut
	bool found = cv::findChessboardCorners(calib_image, pattern_size, corners);

	if(found) {
		std::cout << "Coins de la grille trouvés avec succès.\n";
		// Afficher les coins trouvés sur l'image
		cv::drawChessboardCorners(calib_image, pattern_size, corners, found);
		cv::imshow("Calibration", calib_image);
		cv::waitKey(0);
	} else {
		std::cerr << "Erreur: impossible de trouver les coins de la grille.\n";
	}

	int i = 0;
	for (int it = 0; it < corners.size(); it++)
	{
		// on montre tous les points
		std::cout << "Point numéro : " << i << std::endl;
		std::cout << corners[it] << std::endl;
		std::cout << "" << std::endl;
		i++;
	}
}