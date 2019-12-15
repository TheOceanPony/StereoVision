#include "opencv2/opencv.hpp"
#include "penalty.h"
#include <stdint.h>
#include <iostream>

using namespace cv;

float Scale = 1;
int WindowStartX = 300, WindowStartY = 100, WindowMargin = 10;

int main(int argc, char** argv)
{

	Mat imL = imread("../imgs/bull/im2.ppm", IMREAD_ANYCOLOR);
	Mat imR = imread("../imgs/bull/im6.ppm", IMREAD_ANYCOLOR);
	Mat disp = imread("../imgs/bull/disp2.pgm", IMREAD_ANYDEPTH);
	Mat diffClr, diffGrs;
	std::vector<Mat> BGR_L, BGR_R;

	// Geting image parameters
	int width = imL.cols, height = imL.rows;
	double MAX_DISP, MIN_DISP;
	minMaxLoc(disp, &MIN_DISP, &MAX_DISP);
	std::cout << "Max / Min disparity: " << MAX_DISP << " / " << MIN_DISP << std::endl;

	// Converting color to greyscale
	split(imL, BGR_L);
	split(imR, BGR_R);
	imL = BGR_L[0] * 0.0722f + BGR_L[1] * 0.7152f + BGR_L[2] * 0.2126f;
	imR = BGR_R[0] * 0.0722f + BGR_R[1] * 0.7152f + BGR_R[2] * 0.2126f;

 
	double** BinaryPenalty = getBinaryPenalty(MAX_DISP);
	
	
	for (int r = 0; r < 1; r++)
	{
		for (int c = 0; c < imL.cols; c++)
		{
			//std::cout << unaryPenalty(c, r, 2, imL, imR)<<std::endl;
		}
	}




	namedWindow("Left", WINDOW_FREERATIO);
	namedWindow("Right", WINDOW_FREERATIO);

	imshow("Left", imL);
	imshow("Right", imR);

	resizeWindow("Left", width * Scale, height * Scale);
	resizeWindow("Right", width * Scale, height * Scale);

	moveWindow("Left", WindowStartX, WindowStartY);
	moveWindow("Right", WindowStartX + width * Scale + WindowMargin, WindowStartY);

	waitKey();
}