#ifndef CAPTURE_CAM_HPP
#define CAPTURE_CAM_HPP

#include <opencv2/opencv.hpp>

void capture_cam();

#endif
// fonction qui capture des images depuis la webcam
// retourne le nombre d'images sauvegardées
int capture_cam(cv::Size taille_image);