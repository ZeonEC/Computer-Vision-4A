#include "verif_stereo_calib.hpp"
#include "gestion_XML.hpp"
#include "raw_geometrie_epipolaire.hpp"
#include "disparity.hpp"

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

void show_rect_img(
    const cv::Mat& img1,
    const cv::Mat& img2,
    const cv::Mat& map1x,
    const cv::Mat& map1y,
    const cv::Mat& map2x,
    const cv::Mat& map2y)
{
    if (img1.empty() || img2.empty())
    {
        std::cerr << "Erreur : une des images est vide." << std::endl;
        return;
    }

    cv::Mat rect1, rect2;

    // Application des maps de rectification
    cv::remap(img1, rect1, map1x, map1y, cv::INTER_LINEAR);
    cv::remap(img2, rect2, map2x, map2y, cv::INTER_LINEAR);

    // Dessin de lignes horizontales pour vérifier l'alignement épipolaire
    // Pour l'instant on fait pas mettre un points 
    for (int y = 0; y < rect1.rows; y += 30)
    {
        cv::line(rect1, cv::Point(0, y), cv::Point(rect1.cols - 1, y), cv::Scalar(0, 255, 0), 1);
        cv::line(rect2, cv::Point(0, y), cv::Point(rect2.cols - 1, y), cv::Scalar(0, 255, 0), 1);
    }
    
    //cv::namedWindow("Image rectifiee CAM0", cv::WINDOW_AUTOSIZE);
    //cv::namedWindow("Image rectifiee CAM1", cv::WINDOW_AUTOSIZE);

    cv::Mat recti_img;
    cv::hconcat(rect1, rect2, recti_img);
    cv::imshow("Images rectifiée", recti_img);
    //cv::imshow("Image rectifiee CAM0", rect1);
    //cv::imshow("Image rectifiee CAM1", rect2);

    show_disparity(rect1, rect2);
}

void verif_stereo_calib(int nb_calib) {

    // on récupère les paramètres de calibration stéréo à partir du fichier XML
    cv::Mat camera_matrix1, dist_coeffs1, camera_matrix2, dist_coeffs2, R, T, E, F;
	double rms;
    cv::Size imageSize(600, 600);
    std::string filename = "../../../calibration_images/results/stereo_calibration_result.xml";
	get_stereo_calibration_from_xml(filename, camera_matrix1, dist_coeffs1, camera_matrix2, dist_coeffs2, R, T, E, F,rms);

	std::cout << "Fondamental matrix" << std::endl << F << std::endl;
    // Geometrie épipolaire pré-traitement
	for (int i = 1; i <= 1; ++i) //devrait etre nb_calib mais on laisse pour le test
    {
        std::string img1_path = "../../../calibration_images/calib_cam0_" + std::to_string(i) + ".png";
        std::string img2_path = "../../../calibration_images/calib_cam1_" + std::to_string(i) + ".png";
        cv::Mat img1 = cv::imread(img1_path);
        cv::Mat img2 = cv::imread(img2_path);
        raw_geometrie_epipolaire(img1, img2, F);
    }
    
    // Met les deux images sur le meme plan projectif
	cv::Mat R1, R2, P1, P2, Q;
    cv::stereoRectify(camera_matrix1, dist_coeffs1, camera_matrix2, dist_coeffs2, imageSize, R, T, R1, R2, P1, P2, Q);

	// Calcule les cartes de remapping pour CAM 0 - CAMERA DE GAUCHE
	cv::Mat map1x, map1y;
    cv::initUndistortRectifyMap(camera_matrix1,dist_coeffs1,R1,P1,imageSize,CV_32FC1,map1x,map1y);

    // Calcule les cartes de remapping pour CAM 1 - CAMERA DE DROITE
    cv::Mat map2x, map2y;

    cv::initUndistortRectifyMap(camera_matrix2, dist_coeffs2, R2, P2, imageSize, CV_32FC1, map2x, map2y);

	for (int i = 1; i <= nb_calib; ++i)
    {
        std::string img1_path = "../../../calibration_images/calib_cam0_" + std::to_string(i) + ".png";
        std::string img2_path = "../../../calibration_images/calib_cam1_" + std::to_string(i) + ".png";
        cv::Mat img1 = cv::imread(img1_path);
        cv::Mat img2 = cv::imread(img2_path);
        
        show_rect_img(img1, img2, map1x, map1y, map2x, map2y);
        while (true)
        {
            int key = cv::waitKey(30);

            if (key == 27 || key == 'q' || key == 'Q')
            {
                break;
            }
        }
        cv::destroyAllWindows();
    }
}