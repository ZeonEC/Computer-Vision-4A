#include <iostream>
#include <filesystem>

#include "stereo_capture_cam.hpp"

void stereo_capture_cam(cv::Size taille_image, int& nb_cam) {

    std::vector<cv::VideoCapture> cameras;
    cameras.reserve(std::max(0, nb_cam - 1));

    for (int cur_cam = 0; cur_cam < nb_cam; cur_cam++) {

        cv::VideoCapture cap(cur_cam + 1, cv::CAP_DSHOW);
        if (!cap.isOpened()) {
            std::cerr << "Erreur: impossible d'ouvrir la camera (index 0).\n";
            std::cerr << "Essaie CAP_MSMF ou un autre index (1,2...).\n";
        }
        cameras.push_back(std::move(cap));
    }

    std::vector<std::string> winNames;
    winNames.reserve(cameras.size());

    for (int i = 0; i < cameras.size(); i++) {
        cameras[i].set(cv::CAP_PROP_FRAME_WIDTH, taille_image.width);
        cameras[i].set(cv::CAP_PROP_FRAME_HEIGHT, taille_image.height);

        winNames.push_back("Camera " + std::to_string(i));
        cv::namedWindow(winNames[i], cv::WINDOW_AUTOSIZE);
    }

    std::vector<cv::Mat> frames(cameras.size());

    std::string img_path = "../../../calibration_images/";
    int nb_calib = 0;

    while (true)
    {
        bool ok = true;
        for (int i = 0; i < cameras.size(); i++)
        {
            if (!cameras[i].read(frames[i]) || frames[i].empty())
            {
                std::cerr << "Erreur : frame vide pour la camera " << i << std::endl;
                ok = false;
                break;
            }

            cv::imshow(winNames[i], frames[i]);
        }

        int key = cv::waitKey(1);

        if (!ok)
        {
            if (key == 27) break;
            continue;
        }

        if (key == 27) // ESC
            break;

        if (key == 's' || key == 'S')
        {
            // Copier les images actuelles pour figer la capture
            std::vector<cv::Mat> preview_frames(frames.size());
            for (int i = 0; i < frames.size(); i++)
                {preview_frames[i] = frames[i].clone();}

            // Afficher les previews
            std::vector<std::string> previewNames;
            previewNames.reserve(preview_frames.size());

            for (int i = 0; i < preview_frames.size(); i++)
            {
                std::string previewName = "Preview Camera " + std::to_string(i);
                previewNames.push_back(previewName);
                cv::namedWindow(previewName, cv::WINDOW_AUTOSIZE);
                cv::imshow(previewName, preview_frames[i]);
                cv::moveWindow(previewName, i*800, i*50);
            }

            std::cout << "Appuie sur G pour garder, N pour rejeter, ESC pour quitter." << std::endl;

            // Attendre la décision utilisateur
            while (true)
            {
                int previewKey = cv::waitKey(0);

                if (previewKey == 'g' || previewKey == 'G')
                {
                    nb_calib++;
                    std::cout << "Capture stereo gardee #" << nb_calib << std::endl;

                    for (int i = 0; i < preview_frames.size(); i++)
                    {
                        std::string filename = img_path + "calib_cam" + std::to_string(i)
                            + "_" + std::to_string(nb_calib) + ".png";

                        if (cv::imwrite(filename, preview_frames[i]))
                        {
                            std::cout << "Image sauvegardee : " << filename << std::endl;
                        }
                        else
                        {
                            std::cerr << "Erreur : impossible de sauvegarder " << filename << std::endl;
                        }
                    }
                    break;
                }
                else if (previewKey == 'n' || previewKey == 'N' || previewKey == 'r' || previewKey == 'R')
                {
                    std::cout << "Capture rejetee." << std::endl;
                    break;
                }
                else if (previewKey == 27) // ESC
                {
                    for (const auto& name : previewNames)
                        cv::destroyWindow(name);
                    return;
                }
            }

            // Fermer les previews
            for (const auto& name : previewNames)
            {
                cv::destroyWindow(name);
            }
        }
    }

    for (int i = 0; i < winNames.size(); i++)
    {
        cv::destroyWindow(winNames[i]);
    }
    return;
}