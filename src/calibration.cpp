#include "calibration.hpp"

// On récupère un tableau de points de toute nous images de calibration
std::vector<std::vector<cv::Point2f>> get_grid_points(int nb_calib) {

	// on crée une vecteur de points pour la calibration
	std::vector<cv::Point2f> corners;
	// on crée le tableau de vecteur pour avoir toutes les informations dans une variable
	std::vector<std::vector<cv::Point2f>> images_grid_points;
	// on alloue la place dans le vecteur 
	images_grid_points.reserve(std::max(0, nb_calib - 1));

	cv::Size pattern_size(3, 7); // nombre de coins intérieurs dans la grille (7x9)

	
	for (int i = 1; i < nb_calib+1; i++)
	{
		//    ../../../calibration_images/calib1.jpg
		std::string FILENAME = "../../../calibration_images/calib" + std::to_string(i) + ".jpg";
		std::string WINDOWNAME = "Calibration de l'image numero : " + std::to_string(i);
		// Charger une image de calibration
		cv::Mat calib_image = cv::imread(FILENAME);

		// verifie si l'image a été chargée correctement
		if (calib_image.empty()) {
			std::cerr << "Erreur: impossible de charger l'image de calibration.\n";
			continue;
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
			cv::imshow(WINDOWNAME, calib_image);
			cv::waitKey(0);

			cv::destroyWindow(WINDOWNAME);
		}
		else {
			std::cerr << "Erreur: impossible de trouver les coins de la grille.\n";
		}

		// Transfère des points de l'image 
		images_grid_points.push_back(corners);

		int y = 0;
	}
	return images_grid_points;
}

// Affiche les points de la grille pour chaque image de calibration
void show_grid_points(std::vector<std::vector<cv::Point2f>> &images_grid_points) {
	int i = 1;
	for (const auto& corners : images_grid_points) {
		std::cout << "Image numero : " << i << std::endl;
		int y = 0;
		for (const auto& corner : corners) {
			std::cout << "Point numero : " << y << std::endl;
			std::cout << corner << std::endl;
			std::cout << "" << std::endl;
			y++;
		}
		i++;
	}
}