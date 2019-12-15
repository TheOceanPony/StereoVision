#include "penalty.h"
using namespace cv;

double** getBinaryPenalty(int maxDisp)
{
	// initialisation
	double** binaryPenalty = new double* [maxDisp];
	for (int i = 0; i < maxDisp; i++)
	{
		binaryPenalty[i] = new double[maxDisp];
	}


	// computing every possible disparity
	for (int di = 0; di < maxDisp; di++)
	{
		for (int dj = 0; dj < maxDisp; dj++)
		{
			binaryPenalty[di][dj] = (di - dj)*(di - dj); // TODO shoudI do it the other way? Like, it's simetrical and not random
		}
	}

	return binaryPenalty;
}

int unaryPenalty(int i, int disp, int row, Mat L, Mat R) // Possible mistakes here
{
	int res = ( L.at<uchar>(row, i) - R.at<uchar>(row, i - disp) );

	if(res >= 0 ) return res;
	else return -res;
}