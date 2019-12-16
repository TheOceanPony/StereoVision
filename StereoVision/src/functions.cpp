#include "functions.h"
#include <iostream>

using namespace std;
static int MAX_DISP, WIDTH;

//////////////// Unary penalty /////////////////////
static std::vector<cv::Mat> H;

void initUnaryPenalty(Mat imL, Mat imR, int maxdisp)
{
	int width = imL.cols, height = imL.rows;
	MAX_DISP = maxdisp;
	WIDTH = width;

	for (int row = 0; row < height; row++)
	{
		Mat temp = Mat::zeros(width, MAX_DISP, CV_8UC1);
		for (int i = 0; i < width; i++)
		{
			for (int di = 0; di < MAX_DISP; di++) // TODO Possible mistakes here
			{
				if (i >= di) temp.at<uchar>(i, di) = abs((int)imL.at<uchar>(row, i) - (int)imR.at<uchar>(row, i - di));
				else temp.at<uchar>(i, di) = 255;
			}
		}
		H.push_back(temp);
	}
	std::cout << ">Unary penalty matrix initialised!" << std::endl;
}

int unaryPenalty(int row, int i, int disp)
{
	return (int)H[row].at<uchar>(i, disp);
}


//////////////// Binary penalty /////////////////////
Mat g;

void initBinaryPenalty(int maxdisp)
{
	g = Mat::zeros(maxdisp, maxdisp, CV_8UC1);
	for (int di = 0; di < maxdisp; di++)
	{
		for (int dj = 0; dj < maxdisp; dj++)
		{
			g.at<uchar>(di, dj) = abs(di - dj); // TODO shoudI do it the other way? Like, it's simetrical and not random
		}
	}
	std::cout <<">Binary penalty matrix initialised!"<< std::endl;
}

int binaryPenalty(int di, int dj)
{
	return (int)g.at<uchar>(di, dj);
}


//////////////// Fi /////////////////////
Mat Fi;

int f(int row, int i, int d)
{
	int b = (int)Fi.at<char>(i, d);
	//cout << b << endl;
	if ( b != -1)
	{
		return (int)Fi.at<char>(i, d);
	}
	else
	{
		if (i == 0) return unaryPenalty(row, i, d);

		//min_dt(f)
		int minf = 99999999; // TO DO possible mistakes
		for (int dt = 0; dt < MAX_DISP; dt++)
		{
			int tempf = f(row, i - 1, dt) + binaryPenalty(d, dt);
			if (tempf < minf) minf = tempf;
		}
		return minf + unaryPenalty(row, i, d);
	}
}

void initFi(int row, int m, int maxdisp)
{
	Fi = Mat(m, maxdisp, CV_8S, -1);

	for (int i = 0; i < m; i++)
	{
		for (int d = 0; d < maxdisp; d++)
		{
			Fi.at<char>(i, d) = f(row, i, d);
			//progress(i, m);
		}
	}
	
	namedWindow("Fi", WINDOW_FREERATIO);
	imshow("Left", Fi);
	
}

int minf(int row)
{
	int min = 99999999;
	for (int dt = 0; dt < MAX_DISP; dt++)
	{
		int tempf = (int)Fi.at<char>(WIDTH-1, dt);
		if (tempf < min) min = tempf;
	}
	return min;
}


//////////////// Progress bar /////////////////////
void progress(int p, int max)
{
	system("cls");

	if (p == max)
	{
		std::cout << "Done" << endl;
	}
	else
	{
		int barWidth = 100;
		float progress = p * 100 / max;

		std::cout << "[";
		//int pos = barWidth * progress;
		for (int i = 0; i < barWidth; i++)
		{
			if (i < (int)progress) std::cout << "*";
			else if (i == (int)progress) std::cout << "|";
			else std::cout << " ";
		}
		std::cout << "] - " << p << " / " << max << endl;
	}
	
}