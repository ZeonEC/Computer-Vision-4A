#ifndef CAPTURE_CAM_HPP
#define CAPTURE_CAM_HPP

#include <opencv2/opencv.hpp>
#include <vector>

// fonction qui capture des images depuis la webcam
// retourne le nombre d'images sauvegardées
int capture_cam(cv::Size taille_image, int& cur_cam);

#endif