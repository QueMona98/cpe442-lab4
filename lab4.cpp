/* Lab 4: Sobel Video Playback
Authors: Weston Keitz
         Natalie Tokhmakhian
         Quentin Monasterial
Date: 10/31/2022
Rev 1.0
*/
#include <opencv2/opencv.hpp>
#include <iostream>
#include "lab3funcs.h"
#include <pthread.h>

using namespace cv;
using namespace std;

int main(){
    //Get sobel functions from class
    SobelFunctions SobelObject;
    Mat frame;
    //start video capture
    VideoCapture cap("ocean.mp4");

    while(1){
      //read frame
      cap.read(frame);
      //check if video finished
      if(frame.empty()){
        break;
      }
      //filter and display image
      Mat image = SobelObject.to442_grayscale(frame);
      image = SobelObject.to442_sobel(image);
      imshow("Display image", image);

      if(waitKey(1)==27)
        break; //wait for ESC keystroke in window
    } 
    //close stream
    cap.release();
    destroyAllWindows();
    
    return 0;
}
