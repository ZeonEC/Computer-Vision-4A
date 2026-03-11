#include "calibration.hpp"


// On recupère un tableau de points de toute nous images de calibration
std::vector<std::vector<cv::Point2f>> get_grid_points(int nb_calib) {

	// on cree une vecteur de points pour la calibration
	std::vector<cv::Point2f> corners;
	// on cree le tableau de vecteur pour avoir toutes les informations dans une variable
	std::vector<std::vector<cv::Point2f>> images_grid_points;
	// on alloue la place dans le vecteur 
	images_grid_points.reserve(std::max(0, nb_calib - 1));

	cv::Size pattern_size(7, 9); // nombre de coins interieurs dans la grille (7x9)

	
	for (int i = 1; i < nb_calib+1; i++)
	{
		//    ../../../calibration_images/calib1.jpg
		std::string FILENAME = "../../../calibration_images/calib" + std::to_string(i) + ".png";
		std::string WINDOWNAME = "Calibration de l'image numero : " + std::to_string(i);
		// Charger une image de calibration
		cv::Mat calib_image = cv::imread(FILENAME);

		// verifie si l'image a ete chargee correctement
		if (calib_image.empty()) {
			std::cerr << "Erreur: impossible de charger l'image de calibration.\n";
			std::cerr << "Verifie le chemin et le nom du fichier: " << FILENAME << "\n";
			continue;
		}
		else {
			std::cout << "Image de calibration chargee avec succès.\n";
		}

		// Trouver les coins de la grille, on laisse le FLAGS par defaut
		bool found = cv::findChessboardCorners(calib_image, pattern_size, corners);

		
		if (found) {
			std::cout << "Coins de la grille trouves avec succès.\n";
			// Afficher les coins trouves sur l'image
			cv::drawChessboardCorners(calib_image, pattern_size, corners, found);
			cv::imshow(WINDOWNAME, calib_image);
			cv::waitKey(0);

			cv::destroyWindow(WINDOWNAME);

			// Transfère des points de l'image 
			images_grid_points.push_back(corners);
		}
		else {
			std::cerr << "Erreur: impossible de trouver les coins de la grille.\n";
		}
		int y = 0;
	}
	return images_grid_points;
}

//  Le dérouler de la fonction calibration suit le guide de la documentation de OpenCV pour la fonction calibrateCamera :
// https://docs.opencv.org/4.x/d9/d0c/group__calib3d.html#gaa1e5b8f2c7e7b1a9
// https://docs.opencv.org/4.x/dc/dbb/tutorial_py_calibration.html // MAJORITAIREMENT

void calibration(std::vector<std::vector<cv::Point2f>>& calibration_points, cv::Size& taille_image) {


	//	CREATION DE L'OUTPUT ARRAY OBJECTS_POINTS POUR CALIBRATION
	// 
	// points 3D dans le monde réel de la grille de calibration, on suppose que la grille est sur le plan z=0
	// ils sont espace de 12mm, donc 12*12
	// calcule des points 3D de la grille de calibration
	cv::Size pattern_size(7, 9); // nombre de coins interieurs dans la grille (7x9)
	float square_size = 20.0f; // taille d'un carre en mm
	std::vector<cv::Vec3f> obj; // vecteur pour les points de la grille
	std::vector<std::vector<cv::Vec3f>> obj_points; //vecteur qui contiendra tt nos vecteur obj

	// on remplit le vecteur obj avec les coordonnées 3D de chaque coin de la grille
	for (int i = 0; i < pattern_size.height; i++)
	{
		for (int j = 0; j < pattern_size.width; j++)
		{
			obj.push_back(cv::Vec3f(i * square_size, j * square_size, 0));
		}
	}

	// on remplit le vecteur obj_points avec les mêmes coordonnées 3D pour chaque image de calibration donc ici 20*7*9 (20*64p)
	// ( I1 (grille), I2 (grille), I3 (grille) ... I20 (grille) )
	for (int i = 0; i < calibration_points.size(); i++)
	{
		obj_points.push_back(obj);
	}

	// CREATION DE L'INPUTOUTPUT ARRAY CAMERA_MATRIX (on vas laisser la fonction de calibrage calculer la matrice de la camera)
	cv::Mat camera_matrix;

	// CREATION DE L'INPUTOUTPUT ARRAY DIST_COEFFS (on vas laisser la fonction de calibrage calculer les coefficients de distorsion)
	// on alloue la place pour les coefficients de distorsion (5 coefficients pour le modèle de distorsion radial et tangentiel)
	cv::Mat dist_coeffs;

	// CREATION DE L'OUTPUT ARRAY Rvecs et TVecs (vecteurs de rotation et de translation pour chaque image de calibration)
	std::vector<cv::Mat> rvecs;
	std::vector<cv::Mat> tvecs;

	// CREATION DES OUTPUT ARRAY stdDeviationsIntrensics et stdDeviationsExtrinsics (écart type des paramètres intrinsèques et extrinsèques)
	std::vector<double> stdDeviationsIntrinsics;
	std::vector<double> stdDeviationsExtrinsics;

	// CREATION DE L'OUTPUT ARRAY perViewErrors (erreur de reprojection pour chaque image de calibration)
	std::vector<double> perViewErrors;

	double RMS = cv::calibrateCamera(
		obj_points, calibration_points, taille_image, camera_matrix, dist_coeffs, rvecs,
		tvecs, stdDeviationsExtrinsics, stdDeviationsExtrinsics, perViewErrors);

	std::cout << "Camera Matrix: " << std::endl << camera_matrix << std::endl;
	std::cout << "Distortion Coefficients: " << std::endl << dist_coeffs << std::endl;

	// Pour toutes les images de calibration, affiche les vecteurs de rotation et de translation
	for (int i = 0; i < rvecs.size(); i++) {
		std::cout << "Image " << i + 1 << ":" << std::endl;
		std::cout << "Rotation Vector: " << std::endl << rvecs[i] << std::endl;
		std::cout << "Translation Vector: " << std::endl << tvecs[i] << std::endl;
	}

	// Affiche l'erreur de reprojection moyenne et la "qualité" de la calibration
	if (RMS <= 0.5)
	{
		std::cout << "Calibration terminee avec une erreur de reprojection moyenne de : " << RMS << std::endl;
		std::cout << "Calibration TRES BONNE" << std::endl;
	}
	else if (RMS > 0.5 && RMS <= 1.0)
	{
		std::cout << "Calibration terminee avec une erreur de reprojection moyenne de : " << RMS << std::endl;
		std::cout << "Calibration BONNE" << std::endl;
	}
	else
	{
		std::cout << "Calibration terminee avec une erreur de reprojection moyenne de : " << RMS << std::endl;
		std::cout << "Calibration MAUVAISE" << std::endl;
	}

	show_images_compare(20, camera_matrix, dist_coeffs, taille_image);
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

void show_images_compare(int nb_to_try, cv::Mat camera_matrix, cv::Mat dist_coeffs, cv::Size& taille_image) {

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
	cv::initUndistortRectifyMap(camera_matrix, dist_coeffs,R, camera_matrix, taille_image, m1type, map1, map2 );

	for (int i = 1; i < nb_to_try; i++) {

		std::string original_FILENAME = "../../../calibration_images/calib" + std::to_string(i) + ".png";
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