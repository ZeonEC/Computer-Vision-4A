#include <iostream>           // pour afficher des messages dans le terminal
#include <opencv2/opencv.hpp> // bibliothèque OpenCV (images, calibration, projection)

#include "verification_calibration.hpp"


// ============================================================================
// Fonction : verification_calibration
// Rôle : vérifier la qualité de la calibration de la caméra
//
// Principe :
//   1. charger la matrice de calibration et les coefficients de distorsion
//   2. charger les images du damier
//   3. détecter les coins du damier dans les images
//   4. reprojeter les points 3D théoriques du damier dans l'image
//   5. comparer les points reprojetés avec les points détectés
// ============================================================================

void verification_calibration()
{

    // ------------------------------------------------------------------------
    // Chargement de la calibration depuis le fichier XML
    // ------------------------------------------------------------------------

    std::string filename =
        "../../../calibration_images/results/calibration_cam1_result.xml";

    // ouverture du fichier XML
    cv::FileStorage fs(filename, cv::FileStorage::READ);

    if (!fs.isOpened())
    {
        std::cout << "Impossible d'ouvrir le fichier XML\n";
        return;
    }

    // matrice intrinsèque de la caméra
    cv::Mat camera_matrix;

    // coefficients de distorsion
    cv::Mat dist_coeffs;

    // lecture des données dans le fichier XML
    fs["camera_matrix"] >> camera_matrix;
    fs["dist_coeffs"] >> dist_coeffs;

    // fermeture du fichier
    fs.release();

    std::cout << "Calibration chargee\n";


    // ------------------------------------------------------------------------
    // Paramètres du damier de calibration
    // ------------------------------------------------------------------------

    // nombre de coins internes du damier
    cv::Size pattern_size(7, 9);

    // taille réelle d'un carré du damier (en mm par exemple)
    float square_size = 20.0f;


    // ------------------------------------------------------------------------
    // Création des points 3D du damier (dans le repère du monde)
    // ------------------------------------------------------------------------

    std::vector<cv::Point3f> obj;

    for (int i = 0; i < pattern_size.height; i++)
    {
        for (int j = 0; j < pattern_size.width; j++)
        {
            obj.push_back(
                cv::Point3f(j * square_size, i * square_size, 0)
            );
        }
    }


    // ------------------------------------------------------------------------
    // Boucle sur toutes les images de calibration
    // ------------------------------------------------------------------------

    int i = 1;

    while (true)
    {

        // construction du nom du fichier image
        std::string image_path =
            "../../../calibration_images/calib" +
            std::to_string(i) + ".png";

        // chargement de l'image
        cv::Mat image = cv::imread(image_path);

        // si l'image n'existe plus → fin
        if (image.empty())
            break;


        // --------------------------------------------------------------------
        // Détection des coins du damier dans l'image
        // --------------------------------------------------------------------

        std::vector<cv::Point2f> corners;

        bool found =
            cv::findChessboardCorners(image, pattern_size, corners);


        if (found)
        {

            // ----------------------------------------------------------------
            // Reprojection des points 3D dans l'image
            // ----------------------------------------------------------------

            std::vector<cv::Point2f> reprojected;

            // vecteur rotation et translation (initialisés à zéro ici)
            cv::Mat rvec = cv::Mat::zeros(3, 1, CV_64F);
            cv::Mat tvec = cv::Mat::zeros(3, 1, CV_64F);


            // projection des points 3D vers l'image
            cv::projectPoints(
                obj,            // points 3D du damier
                rvec,           // rotation
                tvec,           // translation
                camera_matrix,  // matrice caméra
                dist_coeffs,    // distorsion
                reprojected     // points projetés
            );


            // ----------------------------------------------------------------
            // Affichage visuel de la comparaison
            // ----------------------------------------------------------------

            for (size_t k = 0; k < corners.size(); k++)
            {
                // coins détectés → vert
                cv::circle(image, corners[k], 4, { 0,255,0 }, -1);

                // coins reprojetés → rouge
                cv::circle(image, reprojected[k], 4, { 0,0,255 }, -1);
            }

            // affichage de l'image avec les deux types de points
            cv::imshow("Verification calibration", image);

            // attendre une touche
            cv::waitKey(0);
        }

        i++;
    }

    // fermeture des fenêtres OpenCV
    cv::destroyAllWindows();
}