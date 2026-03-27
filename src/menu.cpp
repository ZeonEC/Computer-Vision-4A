#include <iostream>

#include "menu.hpp"
#include "capture_cam.hpp"
#include "calibration.hpp"
#include "gestion_XML.hpp"
#include "verification_calibration.hpp"
#include "stereo_capture_cam.hpp"
#include "stereo_calibration.hpp"
#include "verif_stereo_calib.hpp"

///#include "verif_stereo_calib.hpp"

void menu()
{
    cv::Size taille_image = cv::Size(600, 600);
    int choix = 0;
    int nb_cam = 0;
	int nb_calib = 0;


    while (true)
    {
		std::cout << "Que voulez vous faire ? " << std::endl;
		std::cout << " " << std::endl;
		std::cout << "----- MONO CALIBRATION -----" << std::endl;
		std::cout << "1. Effectuer une calibration " << std::endl;
		std::cout << "2. Recuperez une calibration depuis un fichier XML " << std::endl;
		std::cout << "3. Tester la calibration " << std::endl;
		std::cout << " " << std::endl;
		std::cout << "----- STEREO CALIBRATION -----" << std::endl;
		std::cout << "4. Prendre des photos " << std::endl;
		std::cout << "5. stereo-calibration " << std::endl;
		std::cout << "6. Geometrie épipolaire et rectification " << std::endl;
		std::cout << "Autre. Quittez" << std::endl;
		std::cin >> choix;

		if (choix == 1) // L'utilisateur veut faire une calibration
		{
			int choix_photo = 0;
			int nb_to_try = 0;
			std::cout << "Renseignez le nombre de caméras que vous voulez calibrer : " << std::endl;
			std::cin >> nb_cam;


			std::cout << "Voulez vous reprendre des photos ou utilisez celles déja en mémoire ? " << std::endl;
			std::cout << "1. Reprendre des photos " << std::endl;
			std::cout << "2. Garder les anciennes " << std::endl;
			std::cout << "Autre. Quittez" << std::endl;
			std::cin >> choix_photo;

			// Calibration pour les deux cameras
			for (int cur_cam = 0; cur_cam < nb_cam; cur_cam++)
			{
				if (choix_photo == 1) // Oui pour reprendre des photos
				{
					//nb_cam--; // on suppose que les cameras sont indexées à partir de 0, donc on soustrait 1 pour le nombre de caméras à calibrer
					nb_to_try = capture_cam(taille_image, cur_cam);
				}
				else if (choix_photo == 2) // Non pour reprendre des photos 
				{
					std::cout << "Renseignez le nombre d'images de calibration que vous avez déjà prises : " << std::endl;
					std::cin >> nb_to_try;
				}
				else {
					std::cout << "Fin du programme." << std::endl;
					return;
				}
				if (nb_to_try < 1) {
					std::cout << "Nombre d'images de calibration invalide. Fin du programme." << std::endl;
					return;
				}

				// Recuperation de la grille et affichage dans le terminale
				std::vector<std::vector<cv::Point2f>> calibration_points = get_grid_points(nb_to_try, cur_cam);
				show_grid_points(calibration_points);

				calibration(calibration_points, taille_image, cur_cam);
			}
		}

		else if (choix == 2) // L'utilisateur veut recuperer une calibration depuis un fichier XML
		{

			std::cout << "Renseignez le nombre de caméras que vous voulez charger : " << std::endl;
			std::cin >> nb_cam;

			for (int cur_cam = 0; cur_cam < nb_cam; cur_cam++)
			{
				std::string filename = "../../../calibration_images/results/calibration_cam" + std::to_string(cur_cam) + "_result.xml";
				cv::Mat camera_matrix, dist_coeffs;
				std::vector<cv::Mat> rvecs, tvecs;
				std::vector<double> perViewErrors;
				double RMS;
				cv::Size taille_image;
				cv::Size pattern_size;
				float square_size;
				std::vector<std::vector<cv::Point2f>> image_points;

				get_calibration_from_xml(filename,
					camera_matrix, dist_coeffs, rvecs, tvecs, perViewErrors, RMS, taille_image, pattern_size, square_size, image_points);
			}
		}

		else if (choix == 3) // L'utilisateur veut tester la calibration
		{
			std::cout << "Renseignez le nombre de caméras que vous voulez verifier : " << std::endl;
			std::cin >> nb_cam;
			verif_projection(nb_cam);
		}

		// STERO CALIBRATION
		else if (choix == 4) {
			nb_cam = 2;
			stereo_capture_cam(taille_image, nb_cam);
		}

		else if (choix == 5) {
			nb_cam = 2;
			std::cout << "Renseignez le nombre d'images que vous voulez verifier : " << std::endl;
			std::cin >> nb_calib;

			stereo_calibration(nb_calib);
		}

		else if (choix == 6) {
			nb_cam = 2;
			std::cout << "Renseignez le nombre d'images que vous voulez verifier : " << std::endl;
			std::cin >> nb_calib;

			verif_stereo_calib(nb_calib);
		}

		else {
			std::cout << "Arret programme" << std::endl;
			return;
		}
    }
}