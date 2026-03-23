#pragma once

#include <opencv2/opencv.hpp>

// zoom global
extern double zoom;
extern cv::Mat image_original;
extern int mouse_x;
extern int mouse_y;

// fonctions
void onMouse(int event, int x, int y, int flags, void*);
void drawCross(cv::Mat& image, cv::Point2f pt, cv::Scalar color);

void verif_projection(int& nb_cam);
void verification_calibration_stereo();