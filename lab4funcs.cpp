//lab4funcs.cpp

#include <iostream>
#include <opencv2/opencv.hpp>
#include <pthread.h>
#include "lab4funcs.h"

using namespace std;
using namespace cv;


Mat SobelFunctions::to442_grayscale(Mat frame) {
    
    Mat B(frame.size().height,frame.size().width,CV_8U);

    //apply greyscale to all pixels
    for(int i = 0; i<frame.size().width; i++){
        for(int j = 0; j<frame.size().height; j++) {
            Vec3b colors = frame.at<Vec3b>(Point(i,j));
            unsigned char grey = colors.val[0]*0.0722 + colors.val[1]*0.7152 + colors.val[2]*0.2126;
            B.at<unsigned char>(Point(i,j)) = grey;
        }
    }
    return B;
}


Mat SobelFunctions::to442_sobel(Mat C) {
    
    int Gx, Gy, sum;
    Mat D(C.size().height,C.size().width, CV_8U, Scalar(0));

    //apply sobel filter to all non border pixels
    for(int i = 1; i< C.size().width - 1; i++) {
        for(int j = 1; j<C.size().height - 1; j++) {
            Gx = C.at<unsigned char>(Point(i+1,j-1)) + C.at<unsigned char>(Point(i+1,j+1)) - 
                C.at<unsigned char>(Point(i-1,j-1)) - C.at<unsigned char>(Point(i-1,j+1)) +
                2*(C.at<unsigned char>(Point(i+1,j)) - C.at<unsigned char>(Point(i-1,j)));
            
            Gy = C.at<unsigned char>(Point(i-1,j-1)) + C.at<unsigned char>(Point(i+1,j-1)) - 
                C.at<unsigned char>(Point(i-1,j+1)) - C.at<unsigned char>(Point(i+1,j+1)) +
                2*(C.at<unsigned char>(Point(i,j-1)) - C.at<unsigned char>(Point(i,j+1)));
            sum = (abs(Gx) + abs(Gy));

            //cap sum to 8-bit unsigned char
            if(sum > 255){
                sum = 255;
            }    
            D.at<unsigned char>(Point(i,j)) = sum;
        }
    }
    return D;
}

// Mat* SobelFunctions::divideFrame(Mat frame) {

    
// }

Mat SobelFunctions::getQuadrant(Mat frame, int quadrant) {

    switch(quadrant) {
        case 1:
            return getSubFrame(frame, 0, frame.size().height / 2, 0, frame.size().width / 2);
        case 2:
            return getSubFrame(frame, 0, frame.size().height / 2, frame.size().width / 2, frame.size().width - 1);
        case 3:
            return getSubFrame(frame, frame.size().height / 2, frame.size().height - 1, frame.size().width / 2, frame.size().width - 1);
        case 4:
            return getSubFrame(frame, frame.size().height / 2, frame.size().height - 1, 0, frame.size().width / 2);
        default: 
            cerr << "getQuadrant(): Invalid quadrant" << endl;
            exit(1);
    }
}

Mat SobelFunctions::getSubFrame(Mat frame, int startCol, int endCol, int startRow, int endRow) {

    if (startRow >= endRow || startCol >= endCol) {
        cerr << "getSubFrame(): Invalid subframe parameters" << endl;
        exit(1);
    }

    Mat subFrame(endCol- startCol, endRow - startRow, CV_8U, Scalar(0));
    for (int i = 0; i < endRow - startRow; i++) {
        for (int j = 0; j < endCol - startCol; j++) {
            subFrame.at<Vec3b>(Point(i, j)) = frame.at<Vec3b>(Point(i + startRow, j + startCol));
        }
    }
    return subFrame;
}



