#pragma once
#include "opencv2/opencv.hpp"
using namespace cv;

void initUnaryPenalty(Mat imL, Mat imR, int MAX_DISP);
int unaryPenalty(int row, int i, int disp);

void initBinaryPenalty(int MAX_DISP);
int binaryPenalty(int di, int dj);

int f(int row, int i, int d);
void initFi(int row, int m, int maxdisp);

void progress(int i, int max);
