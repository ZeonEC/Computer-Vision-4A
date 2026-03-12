#include <iostream>

// Libs OpenCV
//#include <opencv2/opencv.hpp>
//#include <opencv2/calib3d.hpp>
//#include <opencv2/core.hpp>
//#include <opencv2/highgui.hpp>
//#include <opencv2/improc.hpp>

#include "capture_cam.hpp"
#include "calibration.hpp"

int main() 
{
	std::cout << "Hello, Computer Vision!" << std::endl;
	std::cout << "BRANCH EC_Calibration" << std::endl;

	cv::Size taille_image = cv::Size(600, 600);
	int choix = 0;

	std::cout << "Que voulez vous faire ? " << std::endl;
	std::cout << "1. Effectuer une calibration " << std::endl;
	std::cout << "2. Recuperez une calibration depuis un fichier XML " << std::endl;
	std::cout << "Autre. Quittez" << std::endl;
	std::cin >> choix;

	if (choix == 1) // L'utilisateur veut faire une calibration
	{
		int choix_photo = 0;
		int nb_to_try = 0;
		std::cout << "Voulez vous reprendre des photos ou utilisez celles déja en mémoire ? " << std::endl;
		std::cout << "1. Reprendre des photos " << std::endl;
		std::cout << "2. Garder les anciennes " << std::endl;
		std::cout << "Autre. Quittez" << std::endl;
		std::cin >> choix_photo;

		if(choix_photo == 1) // Oui pour reprendre des photos
		{
			nb_to_try = capture_cam(taille_image);
		}
		else if (choix_photo == 2) // Non pour reprendre des photos 
		{
			std::cout << "Renseignez le nombre d'images de calibration que vous avez déjà prises : " << std::endl;
			std::cin >> nb_to_try;
		}
		else {
			std::cout << "Fin du programme." << std::endl;
			return 0;
		}
		if (nb_to_try < 1) {
			std::cout << "Nombre d'images de calibration invalide. Fin du programme." << std::endl;
			return 0;
		}
		// Recuperation de la grille et affichage dans le terminale
		std::vector<std::vector<cv::Point2f>> calibration_points = get_grid_points(nb_to_try);
		show_grid_points(calibration_points);

		calibration(calibration_points, taille_image);
	}

	if (choix == 2) // L'utilisateur veut recuperer une calibration depuis un fichier XML
	{
	
		std::string filename = "../../../calibration_images/results/calibration_cam" + std::to_string(1) + "_result.xml";
		cv::Mat camera_matrix, dist_coeffs;
		std::vector<cv::Mat> rvecs, tvecs;
		std::vector<double> perViewErrors;
		double RMS;
		get_calibration_from_xml(filename,
			camera_matrix, dist_coeffs, rvecs, tvecs, perViewErrors, RMS, taille_image);
	}
return 0;
}