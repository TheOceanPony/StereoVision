#pragma once
#include "opencv2/opencv.hpp"
using namespace cv;

void initUnaryPenalty(Mat& H, int row, Mat& imL, Mat& imR);


void initBinaryPenalty(Mat &g, float alpha);


void initFi(Mat& Fi, int row, Mat& H, Mat& g);
float f(int row, int i, int d, Mat& Fi, Mat& H, Mat& g);
void initPrevInd(Mat& prevInd, Mat& Fi, Mat &g, int row);

int argmin(int row, int i, Mat& Fi, Mat& g);


void showFl(Mat& M, int size1, int size2);
void showInt(Mat& M, int size1, int size2);
void progress(int p, int max);