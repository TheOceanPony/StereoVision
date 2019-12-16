#include "functions.h"



//////////////// Unary penalty /////////////////////
static std::vector<cv::Mat> H;

void initUnaryPenalty(Mat imL, Mat imR, int MAX_DISP)
{
	int width = imL.cols, height = imL.rows;

	for (int row = 0; row < height; row++)
	{
		Mat temp = Mat::zeros(width, MAX_DISP, CV_8UC1);
		for (int i = 0; i < width; i++)
		{
			for (int di = 0; di < MAX_DISP; di++) // TODO Possible mistakes here
			{
				if (i >= di) temp.at<uchar>(i, di) = abs((int)imL.at<uchar>(row, i) - (int)imR.at<uchar>(row, i - di));
				else temp.at<uchar>(i, di) = 0;
			}
		}
		H.push_back(temp);
	}
}

int unaryPenalty(int row, int i, int disp)
{
	return (int)H[row].at<uchar>(i, disp);
}


//////////////// Binary penalty /////////////////////
Mat g;

void initBinaryPenalty(int MAX_DISP)
{
	g = Mat::zeros(MAX_DISP, MAX_DISP, CV_8UC1);
	for (int di = 0; di < MAX_DISP; di++)
	{
		for (int dj = 0; dj < MAX_DISP; dj++)
		{
			g.at<uchar>(di, dj) = abs(di - dj); // TODO shoudI do it the other way? Like, it's simetrical and not random
		}
	}
}

int initUnaryPenalty(int di, int dj)
{
	return (int)g.at<uchar>(di, dj);
}


//////////////// f /////////////////////
int f()
{
	
}

