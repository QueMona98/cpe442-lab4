/* Lab 4: Sobel Video Playback
Authors: Weston Keitz
         Natalie Tokhmakhian
         Quentin Monasterial
Date: 10/31/2022
Rev 1.0
*/
#include <opencv2/opencv.hpp>
#include <iostream>
#include "lab4funcs.h"
#include <pthread.h>

using namespace cv;
using namespace std;

int main() {

    //Get sobel functions from class
    SobelFunctions SobelObject;
    Mat frame;

    //start video capture
    VideoCapture cap("video.mp4");

    while(1) {
      //read frame
      cap.read(frame);

      //check if video finished
      if(frame.empty()){
        break;
      }

      //filter and display image
      Mat newFrame = Mat(frame.size().height, frame.size().width, CV_8U);
      SobelObject.to442_grayscale(frame, &newFrame, 0, 0.25);
      SobelObject.to442_grayscale(frame, &newFrame, 0.5, 0.75);
      // SobelObject.to442_grayscale(&frame, 0.25, 0.5);
      // frame = SobelObject.to442_sobel(frame, 0, 0.25);
      resize(newFrame, newFrame, Size(newFrame.cols/2, newFrame.rows/2));
      imshow("Display image", newFrame);

      if(waitKey(1)==27)
        break; //wait for ESC keystroke in window
    } 

    //close stream
    cap.release();
    destroyAllWindows();
    
    return 0;
}
