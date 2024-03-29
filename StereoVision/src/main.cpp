#include "opencv2/opencv.hpp"
#include "functions.h"
#include <stdint.h>

//#include <cassert>
//#define assertm(exp, msg) assert(((void)msg, exp))

using namespace cv;

float Scale = 1; // Windows settings
int WindowStartX = 1500, WindowStartY = 100, WindowMargin = 10;

float alpha = 1;

int main(int argc, char** argv)
{
//Tests
    int a[3][3] = { {0, 1, 2},
    {1, 0, 1},
    {2, 1, 0} };

    Mat testBP = Mat(3, 3, CV_32S);
    initBinaryPenalty(testBP, alpha);
    showInt(testBP, 3, 3);

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++) 
        {
            int w = testBP.at<int>(i, j);
            //std::cout <<w << " "<< a[i][j] << " " << i << j << std::endl;
            assert( w == a[i][j] );
        }
    }
    
    Mat imL = imread("../imgs/3d/view2.png", IMREAD_GRAYSCALE);
    Mat imR = imread("../imgs/3d/view1.png", IMREAD_GRAYSCALE);
    //Mat disp = imread("../imgs/sawtooth/disp2.pgm", IMREAD_ANYDEPTH);

    // Geting image parameters
    int width = imL.cols, height = imL.rows;
    //double MAX_DISP;
    //minMaxLoc(disp, nullptr, &MAX_DISP);
   // MAX_DISP = static_cast<int>(MAX_DISP/8);  // TO DO round
    int MAX_DISP = 50;
    
 

    //std::cout << result.at<uchar>(0,2) << std::endl;
    std::cout << "Max disparity: " << MAX_DISP << std::endl;
    std::cout << "Width: " << width<< " Height: " << height << std::endl;

    //Initialisng "g" - binary penalty
    Mat g = Mat(MAX_DISP+1, MAX_DISP+1, CV_32S);  // MAX_DISP, MAX_DISP -1, ... , 1 and 0;

    std::clock_t start, end;
    start = clock();

    initBinaryPenalty(g, alpha);
    //showInt(g, MAX_DISP + 1, MAX_DISP + 1);


    Mat result = Mat(1, height, CV_32S);
    for (int row = 0; row < height; row++)
    {
        progress(row, height);

        //Initialisng "H" - unary penalty
        Mat H = Mat(width, MAX_DISP + 1, CV_32F); 
        initUnaryPenalty(H, row, imL, imR);
        //showFl(H, 20, MAX_DISP + 1);

        Mat Fi = Mat(width, MAX_DISP + 1, CV_32F);
        initFi(Fi, row, H, g);
        //showFl(Fi, 20, MAX_DISP + 1);

        Mat prevInd = Mat(width, MAX_DISP + 1, CV_32S);
        initPrevInd(prevInd, Fi, g, row);
        //showInt(prevInd, width, MAX_DISP + 1);


        //di
        Mat D = Mat(1, width, CV_32S);

        D.at<int>(0, width - 1) = argmin(row, width - 1, Fi, g);

        for (int i = width - 2; i >= 0; i--)
        {
            D.at<int>(0, i) = prevInd.at<int>( i + 1, D.at<int>(0, i + 1) );
        }
        //showInt(D, 1, width);

        for (int i = 0; i < width; i++)
        {
            imR.at<uchar>(row, i) = static_cast<uchar>(D.at<int>(0, i) * 255 / 50);
        } 
    }
    end = clock();
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC); 
    std::cout << "Time taken by program is : " << std::fixed
         << time_taken << std::setprecision(5);
    std::cout << " sec " << std::endl;
  
    std::cout << "Max disparity: " << MAX_DISP << std::endl;
    std::cout << "Width: " << width << " Height: " << height << std::endl;

    namedWindow("Left", WINDOW_FREERATIO);
    namedWindow("Right", WINDOW_FREERATIO);

    imshow("Left", imL);
    imshow("Right", imR);

    //resizeWindow("Left", width * Scale, height * Scale);
    //resizeWindow("Right", width * Scale, height * Scale);
    
    moveWindow("Left", WindowStartX, WindowStartY);
    moveWindow("Right", WindowStartX , WindowStartY);

    waitKey();
}

