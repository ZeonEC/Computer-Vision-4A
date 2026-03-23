#include <iostream>
#include <opencv2/opencv.hpp>

#include "menu.hpp"
#include "capture_cam.hpp"
#include "calibration.hpp"
#include "gestion_XML.hpp"
#include "verification_calibration.hpp"


void menu()
{
    int choix;
    int niveau = 0;

    cv::Size taille_image(600, 600);

    while (true)
    {

        // menu principal
        if (niveau == 0)
        {
            std::cout << "\n===== COMPUTER VISION =====\n";
            std::cout << "1 : Calibration\n";
            std::cout << "2 : Geometrie epipolaire\n";
            std::cout << "3 : Rectification\n";
            std::cout << "4 : Matching\n";
            std::cout << "5 : Reconstruction\n";
            std::cout << "0 : Quitter\n";

            std::cin >> choix;

            if (choix == 1)
                niveau = 1;

            else if (choix == 2)
                std::cout << "Geometrie epipolaire : stéréo calibration\n";
                //verif_stereo_calib();

            else if (choix == 3)
                std::cout << "Rectification : EN ATTENTE\n";

            else if (choix == 4)
                std::cout << "Matching : EN ATTENTE\n";

            else if (choix == 5)
                std::cout << "Reconstruction : EN ATTENTE\n";

            else if (choix == 0)
                break;
        }

        // sous menu calibration
        else if (niveau == 1)
        {
            std::cout << "\n===== MENU CALIBRATION =====\n";
            std::cout << "1 : Prendre photos\n";
            std::cout << "2 : Calibration camera\n";
            std::cout << "3 : Verification calibration\n";
            std::cout << "0 : Retour\n";

            std::cin >> choix;

            if (choix == 1)
                capture_cam(taille_image);

            else if (choix == 2)
                calibration();

            else if (choix == 3)
                verification_calibration();

            else if (choix == 0)
                niveau = 0;
        }

       
    }
}