#include <iostream>           // bibliothèque pour afficher des messages dans le terminal
#include <opencv2/opencv.hpp> // bibliothèque OpenCV (gestion fichiers, matrices, vision)

#include "gestion_XML.hpp" // header contenant la déclaration de la fonction

#include <string>


// Sauvegarde les paramètres de calibration dans un fichier XML

void save_calibration_xml(const std::string& filename,
	const cv::Mat& camera_matrix,
	const cv::Mat& dist_coeffs,
	const std::vector<cv::Mat>& rvecs,
	const std::vector<cv::Mat>& tvecs,
	const std::vector<double>& perViewErrors,
	double RMS,
	const cv::Size& taille_image,
	const cv::Size& pattern_size,
	float square_size,
	const std::vector<std::vector<cv::Point2f>>& image_points)
{
	cv::FileStorage fs(filename, cv::FileStorage::WRITE);

	if (!fs.isOpened()) {
		std::cerr << "Erreur : impossible d'ouvrir le fichier XML en ecriture.\n";
		return;
	}

	fs << "image_width" << taille_image.width;
	fs << "image_height" << taille_image.height;

	fs << "pattern_width" << pattern_size.width;
	fs << "pattern_height" << pattern_size.height;
	fs << "square_size" << square_size;

	fs << "RMS" << RMS;
	fs << "camera_matrix" << camera_matrix;
	fs << "dist_coeffs" << dist_coeffs;

	fs << "rvecs" << "[";
	for (const auto& r : rvecs) {
		fs << r;
	}
	fs << "]";

	fs << "tvecs" << "[";
	for (const auto& t : tvecs) {
		fs << t;
	}
	fs << "]";

	fs << "perViewErrors" << "[";
	for (double e : perViewErrors) {
		fs << e;
	}
	fs << "]";

	fs << "image_points" << "[";
	for (const auto& view_points : image_points) {
		fs << view_points;
	}
	fs << "]";

	fs.release();

	std::cout << "Calibration sauvegardee dans : " << filename << std::endl;
}

// CHARGEMENT DES PARAMETRES DE CALIBRATION DEPUIS UN FICHIER XML

void get_calibration_from_xml(const std::string& filename,
	cv::Mat& camera_matrix,
	cv::Mat& dist_coeffs,
	std::vector<cv::Mat>& rvecs,
	std::vector<cv::Mat>& tvecs,
	std::vector<double>& perViewErrors,
	double& RMS,
	cv::Size& taille_image,
	cv::Size& pattern_size,
	float& square_size,
	std::vector<std::vector<cv::Point2f>>& image_points)
{
	// ouverture du fichier en mode lecture
	cv::FileStorage fs(filename, cv::FileStorage::READ);
	if (!fs.isOpened()) {
		std::cerr << "Erreur : impossible d'ouvrir le fichier XML en lecture.\n";
		return;
	}

	// Nettoyage des vecteurs avant chargement
	rvecs.clear();
	tvecs.clear();
	perViewErrors.clear();
	image_points.clear();

	// Recuperation des valeurs depuis le fichier XML
	fs["image_width"] >> taille_image.width;
	fs["image_height"] >> taille_image.height;

	fs["pattern_width"] >> pattern_size.width;
	fs["pattern_height"] >> pattern_size.height;
	fs["square_size"] >> square_size;

	fs["RMS"] >> RMS;
	fs["camera_matrix"] >> camera_matrix;
	fs["dist_coeffs"] >> dist_coeffs;

	// obliger de faire une boucle pour recuperer les rvecs et tvecs 
	// car ils sont stockés dans un tableau dans le fichier XML
	// ducoup ça reprend pas sous forme de vecteur de Mat mais de tableau de Mat dans le fichier XML
	cv::FileNode rvecs_node = fs["rvecs"];
	for (auto it = rvecs_node.begin(); it != rvecs_node.end(); ++it) {
		cv::Mat r;
		*it >> r;
		rvecs.push_back(r);
	}

	cv::FileNode tvecs_node = fs["tvecs"];
	for (auto it = tvecs_node.begin(); it != tvecs_node.end(); ++it) {
		cv::Mat t;
		*it >> t;
		tvecs.push_back(t);
	}

	cv::FileNode errors_node = fs["perViewErrors"];
	for (auto it = errors_node.begin(); it != errors_node.end(); ++it) {
		double e;
		*it >> e;
		perViewErrors.push_back(e);
	}

	// obliger de faire une boucle pour recuperer les image_points
	// car ils sont stockés comme un tableau de vues dans le fichier XML
	// chaque vue contient un vecteur de cv::Point2f
	cv::FileNode image_points_node = fs["image_points"];
	for (auto it = image_points_node.begin(); it != image_points_node.end(); ++it) {
		std::vector<cv::Point2f> one_view_points;
		*it >> one_view_points;
		image_points.push_back(one_view_points);
	}

	fs.release(); //fermeture

	std::cout << "Calibration chargee depuis : " << filename << std::endl;
}