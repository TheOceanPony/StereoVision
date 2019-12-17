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
		Mat temp = Mat::zeros(width, MAX_DISP, CV_32F);
		for (int i = 0; i < width; i++)
		{
			for (int di = 0; di < MAX_DISP; di++) 
			{
				if (i >= di) temp.at<float>(i, di) = abs((int)imL.at<uchar>(row, i) - (int)imR.at<uchar>(row, i - di));
				else temp.at<float>(i, di) = 0; // TODO Possible mistakes here
			}
		}
		H.push_back(temp);
	}
	std::cout << ">Unary penalty matrix initialised!" << std::endl;
}

int unaryPenalty(int row, int i, int disp)
{
	return H[row].at<float>(i, disp);
}


//////////////// Binary penalty /////////////////////
Mat g;

void initBinaryPenalty(int maxdisp)
{
	g = Mat::zeros(maxdisp, maxdisp, CV_32F);
	for (int di = 0; di < maxdisp; di++)
	{
		for (int dj = 0; dj < maxdisp; dj++)
		{
			g.at<float>(di, dj) = abs(di - dj); 
		}
	}
	std::cout << ">Binary penalty matrix initialised!" << std::endl;
}

int binaryPenalty(int di, int dj)
{
	return g.at<float>(di, dj);
}

//////////////// Fi /////////////////////
Mat Fi;
int HEIGHT;

float f(int row, int i, int d)
{
	if (i == 0) return unaryPenalty(row, i, d);

	float minf = std::numeric_limits<float>::infinity(); 
	for (int dt = 0; dt < MAX_DISP; dt++)
	{
		float tempf = Fi.at<float>(i-1, d) + binaryPenalty(d, dt);
		if (tempf < minf) minf = tempf;
	}
	//std::cout << unaryPenalty(row, i, d) << endl;
	return minf + unaryPenalty(row, i, d);
}

void initFi(int row, int m, int maxdisp)
{
	Fi = Mat(m, maxdisp, CV_32F);
	HEIGHT = m;

	for (int i = 0; i < m; i++)
	{
		for (int d = 0; d < maxdisp; d++)
		{
			Fi.at<float>(i, d) = f(row, i, d);
			//std::cout << Fi.at<float>(i, d) << " - "<< unaryPenalty(row, i, d) << " | ";
		}
		std:cout << endl;
	}
}

int minf(int row)
{
	int min = std::numeric_limits<float>::infinity();
	for (int dt = 0; dt < MAX_DISP; dt++)
	{
		//std::cout << (int)Fi.at<char>(WIDTH - 2, dt);
		float tempf = (int)Fi.at<char>(HEIGHT -1, dt);
		
		if (tempf < min) min = tempf;
	}
	return min;
}

void show()
{
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int di = 0; di < MAX_DISP; di++) // TODO Possible mistakes here
		{
			std::cout << Fi.at<char>(i, di) << " ";
		}
		std::cout << std::endl;
	}
	
}

//////////////// Progress bar /////////////////////
void progress(int p, int max)
{
	system("cls");

	if (p == max-1)
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
		std::cout << std::flush;
	}
	
}