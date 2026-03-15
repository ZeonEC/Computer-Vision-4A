#include <iostream>

#include "menu.hpp"
#include "capture_cam.hpp"
#include "calibration.hpp"
#include "gestion_XML.hpp"
#include "verification_calibration.hpp"

void menu()
{
    int choix;

    while (true)
    {
        std::cout << "\n===== MENU =====\n";
        std::cout << "1 : Prendre photos\n";
        std::cout << "2 : Calibration\n";
        std::cout << "3 : Verification calibration\n";
        std::cout << "4 : Gestion XML\n";
        std::cout << "0 : Quitter\n";

        std::cin >> choix;

        if (choix == 1)
        {
            capture_cam();                // acquisition images
        }

        else if (choix == 2)
        {
            calibration();                // calibration complète
        }

        else if (choix == 3)
        {
            verification_calibration();   // reprojection + erreurs
        }

        else if (choix == 4)
        {
            gestion_XML();                // lecture XML
        }

        else if (choix == 0)
        {
            break;
        }

        else
        {
            std::cout << "Choix invalide\n";
        }
    }
}