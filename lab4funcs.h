//lab3funcs.h
#ifndef LAB3FUNCS_H
#define LAB3FUNCS_H

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class SobelFunctions {
    public:
    Mat to442_grayscale(Mat A);
    Mat to442_sobel(Mat C);
};
#endif // !LAB3FUNCS_H