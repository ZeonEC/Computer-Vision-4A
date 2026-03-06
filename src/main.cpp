#include <iostream>

// Libs OpenCV
#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/improc.hpp>

#include "capture_cam.h"

int main() {
	std::cout << "Hello, Computer Vision!" << std::endl;
	std::cout << "BRANCH EC_Calibration" << std::endl;
	capture_cam();
	return 0;
}
