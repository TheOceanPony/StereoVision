#include "opencv2/opencv.hpp"
#include "functions.h"
#include <stdint.h>
#include <cassert>
#define assertm(exp, msg) assert(((void)msg, exp))

using namespace cv;

float Scale = 1; // Windows settings
int WindowStartX = 1500, WindowStartY = 100, WindowMargin = 10;


int main(int argc, char** argv)
{

    Mat imL = imread("../imgs/sawtooth/im2.ppm", IMREAD_GRAYSCALE);
    Mat imR = imread("../imgs/sawtooth/im6.ppm", IMREAD_GRAYSCALE);
    Mat disp = imread("../imgs/sawtooth/disp2.pgm", IMREAD_ANYDEPTH);

    // Geting image parameters
    int width = imL.cols, height = imL.rows;
    double MAX_DISP;
    minMaxLoc(disp, nullptr, &MAX_DISP);
    MAX_DISP = static_cast<int>(MAX_DISP/8);  // TO DO round

    /*
    assert(2 + 2 == 4);
    std::cout << "Execution continues past the first assert\n";
    assertm(2 + 2 == 5, "There are five lights");
    std::cout << "Execution continues past the second assert\n";
    */

    float f = -0.3f;
    int kl = 4;
    std::cout << abs(f) << std::endl;
    std::cout << "Max disparity: " << MAX_DISP << std::endl;
    std::cout << "Width: " << width<< " Height: " << height << std::endl;

    //Initialisng "g" - binary penalty
    Mat g = Mat(MAX_DISP+1, MAX_DISP+1, CV_32S);  // MAX_DISP, MAX_DISP -1, ... , 1 and 0;

    initBinaryPenalty(g);
    //showInt(g, MAX_DISP + 1, MAX_DISP + 1);

    for (int row = 0; row < 10; row++)
    {
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
        std::cout << D.at<int>(0, width - 1) << std::endl;

        for (int i = width - 2; i >= 0; i--)
        {
            D.at<int>(0, i) = prevInd.at<int>( i + 1, D.at<int>(0, i + 1) );
        }

        showInt(D, 1, width);
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

