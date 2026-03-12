#include <iostream>           // bibliothèque pour afficher des messages dans le terminal
#include <opencv2/opencv.hpp> // bibliothèque OpenCV (gestion fichiers, matrices, vision)

#include "charger_calibration.hpp" // header contenant la déclaration de la fonction


// ============================================================================
// Fonction : charger_calibration
// Rôle : charger les paramètres de calibration d'une caméra depuis un fichier XML
// Le fichier XML contient généralement :
//   - la matrice intrinsèque de la caméra
//   - les coefficients de distorsion
// ============================================================================

void charger_calibration()
{
    // chemin du fichier XML contenant les paramètres de calibration
    std::string filename =
        "../../../calibration_images/results/calibration_cam1_result.xml";

    // ouverture du fichier XML en mode lecture avec OpenCV
    cv::FileStorage fs(filename, cv::FileStorage::READ);

    // vérifie si le fichier a été correctement ouvert
    if (!fs.isOpened())
    {
        std::cout << "Impossible d'ouvrir le XML\n";
        return; // arrêt de la fonction si le fichier n'existe pas
    }

    // matrice intrinsèque de la caméra
    // elle contient la focale et le centre optique
    cv::Mat camera_matrix;

    // coefficients de distorsion de la caméra
    // ils servent à corriger la déformation de l'objectif
    cv::Mat dist_coeffs;

    // lecture des données depuis le fichier XML
    fs["camera_matrix"] >> camera_matrix;
    fs["dist_coeffs"] >> dist_coeffs;

    // fermeture du fichier XML
    fs.release();

    // message indiquant que la calibration a été chargée avec succès
    std::cout << "Calibration chargee depuis : " << filename << std::endl;
}