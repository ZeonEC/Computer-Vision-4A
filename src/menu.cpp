#include <iostream>

#include "menu.hpp"
#include "capture_cam.hpp"
#include "calibration.hpp"
#include "charger_calibration.hpp"

void menu()
{
    int choix;

    cv::Size taille_image(600, 600);

    while (true)
    {
        std::cout << "\n===== MENU =====\n";
        std::cout << "1 : Prendre photos\n";
        std::cout << "2 : Calibration\n";
        std::cout << "3 : Charger calibration\n";
        std::cout << "0 : Quitter\n";

        std::cin >> choix;

        if (choix == 1)
        {
            capture_cam(taille_image);
        }

        else if (choix == 2)
        {
            int nb;

            std::cout << "Nombre d'images calibration : ";
            std::cin >> nb;

            auto pts = get_grid_points(nb);

            show_grid_points(pts);

            calibration(pts, taille_image);
        }

        else if (choix == 3)
        {
            charger_calibration();
        }

        else if (choix == 0)
        {
            break;
        }
    }
}