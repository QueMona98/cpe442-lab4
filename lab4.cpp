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
      // Mat image = SobelObject.to442_grayscale(frame);
      // image = SobelObject.to442_sobel(image);
      // Mat image = frame;
      Mat q1 = SobelObject.getQuadrant(frame, 1);
      Mat q2 = SobelObject.getQuadrant(frame, 2);
      Mat q3 = SobelObject.getQuadrant(frame, 3);
      Mat q4 = SobelObject.getQuadrant(frame, 4);
      // frame = SobelObject.combineFrames(q1, q2, q3, q4);
      imshow("Display image", q4);

      if(waitKey(1)==27)
        break; //wait for ESC keystroke in window
    } 

    //close stream
    cap.release();
    destroyAllWindows();
    
    return 0;
}
