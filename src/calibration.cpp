#include <iostream>          // bibliothèque pour afficher du texte dans le terminal
#include <opencv2/opencv.hpp> // bibliothèque principale OpenCV (images, matrices, fonctions vision)

#include "calibration.hpp"   // header contenant les prototypes des fonctions de calibration


// ============================================================================
// Fonction : get_grid_points
// Rôle : détecter les coins du damier dans toutes les images de calibration
// Entrée : nb_calib → nombre d'images de calibration à analyser
// Sortie : un vecteur contenant les coordonnées des coins détectés pour chaque image
// ============================================================================

std::vector<std::vector<cv::Point2f>> get_grid_points(int nb_calib)
{
    // tableau contenant les coins détectés pour toutes les images
    std::vector<std::vector<cv::Point2f>> images_grid_points;

    // taille du damier : nombre de coins internes (colonnes, lignes)
	// à voir 
    cv::Size pattern_size(7, 9);

    // boucle sur toutes les images de calibration
    for (int i = 1; i <= nb_calib; i++)
    {
        // construction du nom du fichier image
        std::string filename =
            "../../../calibration_images/calib" +
            std::to_string(i) + ".png";

        // chargement de l'image
        cv::Mat image = cv::imread(filename);

        // si l'image n'existe pas ou ne s'ouvre pas → on passe à la suivante
        if (image.empty())
            continue;

        // vecteur qui contiendra les coins détectés dans cette image
        std::vector<cv::Point2f> corners;

        // recherche automatique des coins du damier dans l'image
        bool found =
            cv::findChessboardCorners(image, pattern_size, corners);

        // si les coins sont détectés
        if (found)
        {
            // on ajoute les coins trouvés dans la liste globale
            images_grid_points.push_back(corners);

            // dessine les coins détectés sur l'image (pour visualisation)
            cv::drawChessboardCorners(image, pattern_size, corners, found);

            // affiche l'image avec les coins détectés
            cv::imshow("Corners", image);

            // pause de 500 ms pour voir le résultat
            cv::waitKey(500);
        }
    }

    // fermeture des fenêtres OpenCV
    cv::destroyAllWindows();

    // retourne tous les points détectés
    return images_grid_points;
}


// ============================================================================
// Fonction : show_grid_points
// Rôle : afficher dans le terminal les coordonnées des coins détectés
// Entrée : vecteur contenant les coins pour chaque image
// ============================================================================

void show_grid_points(
    std::vector<std::vector<cv::Point2f>>& images_grid_points)
{
    int i = 1;

    // parcours de chaque image
    for (const auto& corners : images_grid_points)
    {
        std::cout << "Image " << i << std::endl;

        // parcours de tous les coins détectés dans l'image
        for (const auto& p : corners)

            // affichage de la position du coin (x,y)
            std::cout << p << std::endl;

        i++;
    }
}


// ============================================================================
// Fonction : calibration
// Rôle : lancer la calibration de la caméra à partir des points détectés
// Entrées :
//   calibration_points → coins du damier détectés dans les images
//   taille_image → taille des images utilisées
// ============================================================================

void calibration(
    std::vector<std::vector<cv::Point2f>>& calibration_points,
    cv::Size& taille_image)
{
    // message indiquant que la calibration commence
    std::cout << "Calibration lancee..." << std::endl;

    // affiche le nombre d'images utilisées pour la calibration
    std::cout << "Nombre d'images utilisees : "
        << calibration_points.size()
        << std::endl;

    // NOTE :
    // Ici on devrait normalement appeler la fonction OpenCV :
    // cv::calibrateCamera()
    // pour calculer :
    // - la matrice intrinsèque de la caméra
    // - les coefficients de distorsion
}