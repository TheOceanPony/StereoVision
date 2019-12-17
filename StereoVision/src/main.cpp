#include "opencv2/opencv.hpp"
#include "functions.h"
#include <stdint.h>
#include <conio.h>

using namespace cv;

float Scale = 1; // Windows settings
int WindowStartX = 1500, WindowStartY = 100, WindowMargin = 10;


int main(int argc, char** argv)
{

	Mat imL = imread("../imgs/sawtooth/im2.ppm", IMREAD_ANYCOLOR);
	Mat imR = imread("../imgs/sawtooth/im6.ppm", IMREAD_ANYCOLOR);
	Mat disp = imread("../imgs/sawtooth/disp2.pgm", IMREAD_ANYDEPTH);
	Mat diffClr, diffGrs;
	std::vector<Mat> BGR_L, BGR_R;



	// Geting image parameters
	int width = imL.cols, height = imL.rows;
	double max, MIN_DISP;
	minMaxLoc(disp, &MIN_DISP, &max);
	int MAX_DISP = max/8;
	std::cout << "Max / Min disparity: " << MAX_DISP << " / " << MIN_DISP << std::endl;
	std::cout << "Width: " << width<< " Height: " << height << std::endl;

	Mat test = Mat::zeros(100, 10, CV_8UC1);
	std::cout << "Width: " << test.cols << " Height: " << test.rows << std::endl; //For reference on args positions

	// Converting color to greyscale
	split(imL, BGR_L);
	split(imR, BGR_R);
	imL = BGR_L[0] * 0.0722f + BGR_L[1] * 0.7152f + BGR_L[2] * 0.2126f;
	imR = BGR_R[0] * 0.0722f + BGR_R[1] * 0.7152f + BGR_R[2] * 0.2126f;

	//Mat A = Mat(20, 20, CV_8S, -1);

	float a = std::numeric_limits<float>::infinity();
	std::cout << a << std::endl;
	

	initUnaryPenalty(imL, imR, MAX_DISP);
	initBinaryPenalty(MAX_DISP);

	
	
	for (int row = 0; row < 1 ; row++)
	{
		initFi(row, height, MAX_DISP);
		//progress(row, height);
		//std::cout << minf(row) << std::endl<<std::flush;
	}
	//show();
	

	namedWindow("Left", WINDOW_FREERATIO);
	namedWindow("Right", WINDOW_FREERATIO);
	
	//namedWindow("unaryPen[1]", WINDOW_FREERATIO);

	imshow("Left", imL);
	imshow("Right", imR);
	//imshow("unaryPen[0]", Fi);
	//imshow("unaryPen[1]", unaryPen[0]);

	resizeWindow("Left", width * Scale, height * Scale);
	resizeWindow("Right", width * Scale, height * Scale);
	//resizeWindow("unaryPen[0]", unaryPen[0].cols * Scale, unaryPen[0].rows * Scale);
	//resizeWindow("unaryPen[1]", unaryPen[1].cols * Scale, unaryPen[1].rows * Scale);

	moveWindow("Left", WindowStartX, WindowStartY);
	moveWindow("Right", WindowStartX + width * Scale + WindowMargin, WindowStartY);
	//moveWindow("unaryPen[0]", WindowStartX, WindowStartY + height * Scale + WindowMargin + 30);
	//moveWindow("unaryPen[1]", WindowStartX + width * Scale + WindowMargin, WindowStartY + height * Scale + WindowMargin + 30);

	waitKey();
}

