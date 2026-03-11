#include <iostream>

#include "menu.hpp"
#include "capture_cam.hpp"
#include "calibration.hpp"

void menu()
{
    int choix;

    // boucle infini pour afficher le menu
    while (true)
    {
        std::cout << "\n===== MENU =====\n";
        std::cout << "1 : Prendre des photos\n";
        std::cout << "2 : Calibration\n";
        std::cout << "0 : Quitter\n";
        std::cout << "Choix : ";

        // lecture du choix
        std::cin >> choix;

        if (choix == 1)
        {
            capture_cam();
        }
        else if (choix == 2)
        {
            calibration();
        }
        else if (choix == 0)
        {
            break; //return
        }
        else
        {
            std::cout << "Choix invalide\n";
        }
    }
}