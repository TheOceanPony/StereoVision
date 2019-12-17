#pragma once
#include "opencv2/opencv.hpp"
using namespace cv;

void initUnaryPenalty(Mat& H, int row, Mat& imL, Mat& imR);
//int unaryPenalty(int row, int i, int disp);

void initBinaryPenalty(Mat &g);
//int binaryPenalty(int di, int dj);

void initFi(Mat& Fi, int row, Mat& H, Mat& g);
float f(int row, int i, int d, Mat& Fi, Mat& H, Mat& g);
void initPrevInd(Mat& prevInd, Mat& Fi, Mat &g, int row);

//void progress(int i, int max);
void showFl(Mat& M, int size1, int size2);
void showInt(Mat& M, int size1, int size2);