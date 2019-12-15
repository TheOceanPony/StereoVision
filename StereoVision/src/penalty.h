#pragma once
#include "opencv2/opencv.hpp"

double** getBinaryPenalty(int maxDisp);

int unaryPenalty(int i, int disp, int row, cv::Mat L, cv::Mat R);