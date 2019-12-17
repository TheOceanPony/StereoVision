#include "functions.h"
#include <iostream>

using namespace std;

//////////////// Unary penalty /////////////////////
void initUnaryPenalty(Mat &H, int row, Mat& imL, Mat& imR)
{
    int maxdi = H.cols, width = H.rows;
    std::cout << "cols / rows :"<<width<<" "<<maxdi << std::endl;
    for (int i = 0; i < width; i++)
    {
        for (int di = 0; di < maxdi; di++)
        {
            if (i >= di)
                H.at<float>(i, di) =
                abs (static_cast<float>(imL.at<uchar>(row, i)) - static_cast<float>(imR.at<uchar>(row, i - di)) );
            else
                H.at<float>(i, di) = std::numeric_limits<float>::infinity();
        }
    }
    std::cout << ">Unary penalty matrix for row "<<row<<" initialised!" << std::endl;
}

void showFl(Mat& M, int size1, int size2)
{
    std::cout << std::endl;
    for (int i = 0; i < size1; i++)
    {
        for (int j = 0; j < size2; j++) // TODO Possible mistakes here
        {
            std::cout << M.at<float>(i, j) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}



//////////////// Binary penalty /////////////////////

void initBinaryPenalty(Mat &g)
{
    int maxdi = g.cols;
    for (int di = 0; di < maxdi; di++)
    {
        for (int dj = 0; dj < maxdi; dj++)
        {
            g.at<int>(di, dj) = abs(di - dj);
        }
    }
    std::cout << ">Binary penalty matrix initialised!" << std::endl;
}

void showInt(Mat& M, int size1, int size2)
{
    std::cout << std::endl;
    for (int i = 0; i < size1; i++)
    {
        for (int j = 0; j < size2; j++) // TODO Possible mistakes here
        {
            std::cout << M.at<int>(i, j) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}



//////////////// Ñumulative penalty /////////////////////
void initFi(Mat& Fi, int row, Mat& H, Mat& g)
{
    int width = Fi.rows, maxd = Fi.cols;
    for (int i = 0; i < width; i++)
    {
        for (int d = 0; d < maxd; d++)
        {
            Fi.at<float>(i, d) = f(row, i, d, Fi, H, g);
        }
    }
}

float f(int row, int i, int d, Mat &Fi, Mat &H, Mat &g)
{
    int maxd = g.cols;
    if (i == 0) return H.at<float>(i, d);

    else
    {
        float minf = std::numeric_limits<float>::infinity();
        for (int dj = 0; dj < maxd; dj++)
        {
            float temp = Fi.at<float>(i - 1, dj) + g.at<int>(d, dj);
            if (temp < minf) minf = temp;
        }
        return minf + H.at<float>(i, d);
    }
}



//////////////// Previous index /////////////////////
void initPrevInd(Mat &prevInd, Mat &Fi, Mat &g, int row)
{
    int width = prevInd.rows, maxd = prevInd.cols;

    for (int d = 0; d < maxd; d++)
    {
        prevInd.at<int>(0, d) = 0;
    }

    for (int i = 1; i < width; i++) 
    {
        for (int d = 0; d < maxd; d++)
        {
            // argmin_dj ( f_i-1(dj) + g(d, dj) )
            float minf = std::numeric_limits<float>::infinity();
            int mind = 0;
            for (int dj = 0; dj < maxd; dj++)
            {
                float temp = Fi.at<float>(i - 1, dj) + static_cast<float>(g.at<int>(dj, d));
                if (temp < minf)
                {
                    minf = temp;
                    mind = dj;
                }
            }
            prevInd.at<int>(i, d) = mind;
        }
    }
}
