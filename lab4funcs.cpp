//lab4funcs.cpp

#include <iostream>
#include <opencv2/opencv.hpp>
#include <pthread.h>
#include "lab4funcs.h"

using namespace std;
using namespace cv;

Mat SobelFunctions::to442_grayscale(Mat frame) {
    
    Mat B(frame.size().height,frame.size().width, CV_8U, Scalar(0));

    //apply greyscale to all pixels
    for(int i = 0; i < frame.size().width - 1; i++){
        for(int j = 0; j < frame.size().height - 1; j++) {
            Vec3b colors = frame.at<Vec3b>(j, i);
            uchar grey = colors.val[0]*0.0722 + colors.val[1]*0.7152 + colors.val[2]*0.2126;
            B.at<uchar>(j, i) = grey;
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

Mat SobelFunctions::combineFrames(Mat q1, Mat q2, Mat q3, Mat q4) {
    Mat combinedFrames(q1.size().height + q4.size().height, q1.size().width + q2.size().width, CV_8UC3, Scalar(0));

    for (int i = 0; i < combinedFrames.size().width - 1; i++) {
        for (int j = 0; j < combinedFrames.size().height - 1; j++) {

            if (i < combinedFrames.size().width / 2 && j < combinedFrames.size().height / 2) {  // if quadrant 1
                combinedFrames.at<Vec3b>(j, i) = q1.at<Vec3b>(j, i);
            }
            else if (i >= combinedFrames.size().width / 2 && j < combinedFrames.size().height / 2) {   // if quadrant 2
                combinedFrames.at<Vec3b>(j, i) = q2.at<Vec3b>(j, i - q2.size().width);
            }
            else if (i >= combinedFrames.size().width / 2 && j >= combinedFrames.size().height / 2) {    // if quadrant 3
                combinedFrames.at<Vec3b>(j, i) = q3.at<Vec3b>(j - q3.size().height, i - q3.size().width);
            }
            else if (i < combinedFrames.size().width / 2 && j >= combinedFrames.size().height / 2) {  // if quadrant 4
                combinedFrames.at<Vec3b>(j,i) = q4.at<Vec3b>(j - q4.size().height, i);
            }
        }
    }

    return combinedFrames;
}

Mat SobelFunctions::getQuadrant(Mat frame, int quadrant) {

    switch(quadrant) {
        case 1:
            return getSubFrame(frame, 0, (frame.size().height / 2) - 1, 0, (frame.size().width / 2) - 1);
        case 2:
            return getSubFrame(frame, 0, (frame.size().height / 2) - 1, frame.size().width / 2, frame.size().width - 1);
        case 3:
            return getSubFrame(frame, frame.size().height / 2, frame.size().height - 1, frame.size().width / 2, frame.size().width - 1);
        case 4:
            return getSubFrame(frame, frame.size().height / 2, frame.size().height - 1, 0, (frame.size().width / 2) - 1);
        default: 
            cerr << "getQuadrant(): Invalid quadrant" << endl;
            exit(1);
    }
}

Mat SobelFunctions::getSubFrame(Mat frame, int startRow, int endRow, int startCol, int endCol) {

    if (startRow >= endRow || startCol >= endCol) {
        cerr << "getSubFrame(): Invalid subframe parameters" << endl;
        exit(1);
    }

    Mat subFrame(endRow - startRow + 1, endCol- startCol + 1, CV_8UC3, Scalar(0));
    for (int i = 0; i < subFrame.size().width - 1; i++) {
        for (int j = 0; j < subFrame.size().height - 1; j++) {
            subFrame.at<Vec3b>(j, i) = frame.at<Vec3b>(j + startRow, i + startCol);
        }
    }
    return subFrame;
}



