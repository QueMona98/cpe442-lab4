#ifndef LAB3FUNCS_H
#define LAB3FUNCS_H

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class SobelFunctions {
    private:
    Mat getSubFrame(Mat frame, int startRow, int endRow, int startCol, int endCol);

    public:
    Mat to442_grayscale(Mat A);
    void to442_grayscale(Mat frame, Mat *newFrame, double a, double b);
    Mat to442_sobel(Mat C);
    void to442_sobel(Mat *C, double a, double b);
    Mat getQuadrant(Mat frame, int quadrant);
    Mat combineFrames(Mat q1, Mat q2, Mat q3, Mat q4);

};
#endif 