#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>

#include "calibration.hpp"

void calibration()
{
    std::string images_path = "../../../calibration_images/"; 

    // taille du damier en nombre de coin interne
    // 4 coins horizontaux et 3 coins verticaux
    cv::Size patternSize(4, 3);

    for (int i = 1; i <= 20; i++)   // 20 images
    {
        // construction du nom du fichier
        std::string filename = images_path + "calib" + std::to_string(i) + ".jpg";

        // lecture de l'image depuis le disque
        cv::Mat image = cv::imread(filename);

        if (image.empty())
        {
            std::cout << "Fin des images\n";
            break;
        }

        // vecteur contenant les positions des coins détectés
        std::vector<cv::Point2f> corners;

        // fonction OpenCV qui détecte les coins du damier 
        bool found = cv::findChessboardCorners(image, patternSize, corners);

        if (found)
        {
            std::cout << "Damier detecte dans " << filename << "\n";
            // dessin des coins détectés sur l'image
            cv::drawChessboardCorners(image, patternSize, corners, found);
        }
        else
        {
            std::cout << "Damier non detecte dans " << filename << "\n";
        }

        // Affiche le résultat 
        cv::imshow("Calibration", image);

        // si la fenêtre est fermée avec la croix
        if (cv::getWindowProperty("Calibration", cv::WND_PROP_VISIBLE) < 1)
            break;

        int key = cv::waitKey(0); 

        // ESC pour arrêter 
        if (key == 27)
            break;
    }

    cv::destroyAllWindows();
}