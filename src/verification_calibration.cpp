#include <iostream>           // pour afficher des messages dans le terminal
#include <opencv2/opencv.hpp> // bibliothèque OpenCV (images, calibration, projection)

#include "verification_calibration.hpp"
#include "gestion_XML.hpp" // pour récupérer les données de calibration depuis le fichier XML

// Test log dans fichier texte :
#include <fstream>

// verification de la calibration de la caméra en comparant les paramètres obtenus avec calibrateCamera et projectPoint()

void verif_projection(int& nb_cam) {

	//-------------------------------------
	/// Mise a dispo dans un fichier texte
	std::ofstream logfile("../../../calibration_images/results/verification_projection.txt");

	if (!logfile.is_open()) {
		std::cout << "Impossible de creer le fichier log" << std::endl;
		return;
	}
	// sauvegarde du buffer console
	std::streambuf* cout_buffer = std::cout.rdbuf();
	// redirection vers fichier
	std::cout.rdbuf(logfile.rdbuf());
	//-------------------------------------



	for (int cur_cam = 0; cur_cam < nb_cam; cur_cam++)
	{
		// Recupération de toutes les valeurs de la calibration depuis le fichier XML
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


		double fx = camera_matrix.at<double>(0, 0); // fx
		double fy = camera_matrix.at<double>(1, 1); // fy
		double focal_diff = std::abs(fx - fy);

		std::cout << "" << std::endl;
		std::cout << "==================================================" << std::endl;
		std::cout << "Verification de la camera : " << cur_cam << std::endl;
		std::cout << "==================================================" << std::endl;
		std::cout << "__________________TEST DES FOCALES__________________" << std::endl;
		std::cout << "Verification de fx et fy (valeurs en pixels): " << std::endl;
		std::cout << "Difference entre les deux focales : " << focal_diff << std::endl;


		// verification de cx, cy

		double cx = camera_matrix.at<double>(0, 2); // cx de la matrice intrinsèque
		double cy = camera_matrix.at<double>(1, 2); // cy de la matrice intrinsèque
		double center_x = taille_image.width / 2.0; // vrai centre
		double center_y = taille_image.height / 2.0; // vrai centre

		double cx_diff = std::abs(cx - center_x);
		double cy_diff = std::abs(cy - center_y);

		std::cout << "__________________TEST DU POINT PRINCIPALE__________________" << std::endl;
		std::cout << "Verification de cx et cy (valeurs en pixels): " << std::endl;
		std::cout << "Difference entre les deux cx : " << cx_diff << std::endl;
		std::cout << "Difference entre les deux cy : " << cy_diff << std::endl;

		// reprojection

		std::cout << "__________________TEST DE REPROJECTION__________________" << std::endl;

		// reconstruction des object_points
		std::vector<cv::Point3f> obj;
		std::vector<std::vector<cv::Point3f>> object_points;

		for (int i = 0; i < pattern_size.height; i++)
		{
			for (int j = 0; j < pattern_size.width; j++)
			{
				obj.push_back(cv::Point3f(i * square_size, j * square_size, 0.0f));
			}
		}

		for (size_t i = 0; i < image_points.size(); i++)
		{
			object_points.push_back(obj);
		}

		// On crée un nouveau tableau de points projetés pour chaque image de calibration
		std::vector<cv::Point2f> projected_points;
		double rms_error = 0.0;

		for (int i = 0; i < image_points.size(); i++)
		{
			for (int j = 0; j < image_points[i].size(); j++)
			{
				cv::projectPoints(object_points[i], rvecs[i], tvecs[i], camera_matrix, dist_coeffs, projected_points);
				double error_x = std::abs(image_points[i][j].x - projected_points[j].x);
				double error_y = std::abs(image_points[i][j].y - projected_points[j].y);

				std::cout << "Erreur de reprojection pour l'image en X " << i + 1 << " : " << error_x << std::endl;
				std::cout << "Erreur de reprojection pour l'image en Y " << i + 1 << " : " << error_x << std::endl;

				rms_error += error_x * error_x + error_y * error_y;
			}
		}
		// comparaison
		rms_error = std::sqrt(rms_error / (image_points.size() * pattern_size.width * pattern_size.height));
		double rms_compare = std::abs(RMS - rms_error);

		std::cout << "__________________COMPARAISON__________________" << std::endl;
		std::cout << "RMS lu depuis le XML : " << RMS << std::endl;
		std::cout << "RMS calculé à partir de projectPoints : " << rms_error << std::endl;
		std::cout << "Difference entre les deux RMS : " << rms_compare << std::endl;

		// conclusion
		std::cout << "__________________CONCLUSION__________________" << std::endl;


		//-------------------------------------
		/// Mise a dispo dasn un fichier texte
		// restaurer la console
		std::cout.rdbuf(cout_buffer);
		logfile.close();
		//-------------------------------------
	}
}

void verif_distortion(int nb_to_try, cv::Mat camera_matrix, cv::Mat dist_coeffs, cv::Size& taille_image, int cur_cam) {

	// on utilisera R pour la stéréo rectification de l'image
	// pour l'instant elle peut être vide.
	cv::Mat R;

	// Le deuxieme argument camera_matrix reste le même que le premier argument, 
	// car on ne change pas la matrice de la camera pour la stéréo rectification ou en utilisant getOptimalNewCameraMatrix
	// a chanegr plus tard si on veut faire de la stéréo

	int m1type = CV_32FC1; // type de la map pour la stéréo rectification
	cv::Mat  map1, map2; // maps pour la stéréo rectification

	// L'utilisation de initUndistortRectifyMap et remap est supposé plus performante.
	// cela permet de faire le calcul de la correction de distorsion une seule fois 
	// et de l'appliquer à toutes les images de calibration, au lieu de recalculer la correction pour chaque image 
	// avec undistort.
	cv::initUndistortRectifyMap(camera_matrix, dist_coeffs, R, camera_matrix, taille_image, m1type, map1, map2);

	for (int i = 1; i < nb_to_try + 1; i++) {

		std::string original_FILENAME = "../../../calibration_images/calib_cam" + std::to_string(cur_cam) + "_" + std::to_string(i) + ".png";
		std::string original_WINDOWNAME = "Calibration de l'image numero : " + std::to_string(i);
		cv::Mat original_image = cv::imread(original_FILENAME);
		cv::imshow(original_WINDOWNAME, original_image);


		// on undistord l'image de calibration en utilisant les paramètres de calibration obtenus
		// transforme ces paramètres en correspondance pixel et applique la correction de distorsion à l'image de calibration
		//
		// Originale : si on veut pas s'embeterinitUndistortRectifyMap et remap
		// cv::Mat undistorted_image;
		//cv::undistort(original_image, undistorted_image, camera_matrix, dist_coeffs);

		cv::Mat undistorted_image;
		cv::remap(original_image, undistorted_image, map1, map2, cv::INTER_LINEAR);
		cv::imshow("Image undistordue de la calibration numero : " + std::to_string(i), undistorted_image);

		cv::waitKey(0);
		cv::destroyAllWindows();
	}
	cv::destroyAllWindows();
}