#include "calibration.h"

// On récupère un tableau de points de toute nous images de calibration
std::vector<std::vector<cv::Point2f>> get_grid_points(int nb_calib) {

	// on crée une vecteur de points pour la calibration
	std::vector<cv::Point2f> corners;
	// on crée le tableau de vecteur pour avoir toutes les informations dans une variable
	std::vector<std::vector<cv::Point2f>> images_grid_points;

	cv::Size pattern_size(3, 7); // nombre de coins intérieurs dans la grille (7x9)

	
	for (int i = 1; i < nb_calib; i++)
	{
		//    ../../../calibration_images/calib1.jpg
		std::string FILENAME = "../../../calibration_images/calib" + std::to_string(i) + ".jpg";
		// Charger une image de calibration
		cv::Mat calib_image = cv::imread(FILENAME);

		// verifie si l'image a été chargée correctement
		if (calib_image.empty()) {
			std::cerr << "Erreur: impossible de charger l'image de calibration.\n";
			break;
		}
		else {
			std::cout << "Image de calibration chargée avec succès.\n";
		}

		// Trouver les coins de la grille, on laisse le FLAGS par défaut
		bool found = cv::findChessboardCorners(calib_image, pattern_size, corners);

		if (found) {
			std::cout << "Coins de la grille trouvés avec succès.\n";
			// Afficher les coins trouvés sur l'image
			cv::drawChessboardCorners(calib_image, pattern_size, corners, found);
			cv::imshow("Calibration de l'image numéro : " + std::to_string(i), calib_image);
			cv::waitKey(0);
		}
		else {
			std::cerr << "Erreur: impossible de trouver les coins de la grille.\n";
		}

		// Transfère des points de l'image 
		images_grid_points[i - 1] = corners;

		int y = 0;
		for (int it = 0; it < corners.size(); it++)
		{
			// on montre tous les points
			std::cout << "Point numéro : " << y << std::endl;
			std::cout << corners[it] << std::endl;
			std::cout << "" << std::endl;
			y++;
		}
	}
	return images_grid_points;
}