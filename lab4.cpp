/* Lab 4: Sobel Video Playback
Authors: Weston Keitz
         Natalie Tokhmakhian
         Quentin Monasterial
Date: 10/31/2022
Rev 1.0
*/
#include <opencv2/opencv.hpp>
#include <iostream>
#include <pthread.h>

using namespace cv;
using namespace std;

pthread_t thread[4];

pthread_barrier_t barrier;

void *thread0Status;

typedef struct threadArgs{
    Mat* inputFrame;
    Mat* procFrame;
    int rows;
    int cols;
    
    int start;
    int stop; 
} threadStruct;


void* grayscale_sobel(void* threadArgs);

void setupFunction(int numThreads){
  pthread_barrier_init(&barrier, NULL, numThreads);
}
int main() {
    int numThreads = 5; //main and thread
   

  //  pthread_attr_t attr;
   // pthread_attr_init(&attr);
    //pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  
    VideoCapture cap("ocean.mp4");
    
    Mat frame;

    if(!cap.isOpened()){
        return 0;
    }

 
    cap >> frame;
 
    Mat displayFrame = Mat(frame.size().height,frame.size().width,CV_8UC1,Scalar::all(0));

    threadStruct thread0 = {.inputFrame = &frame, .procFrame = &displayFrame, 
                      .rows = frame.size().width, .cols = frame.size().height, 
                      .start = 0, .stop = (frame.size().height/4) + 1};

    threadStruct thread1 = {.inputFrame = &frame, .procFrame = &displayFrame, 
                      .rows = frame.size().width, .cols = frame.size().height, 
                      .start = (frame.size().height/4) - 1, .stop = (frame.size().height/2) + 1};

    threadStruct thread2 = {.inputFrame = &frame, .procFrame = &displayFrame, 
                      .rows = frame.size().width, .cols = frame.size().height, 
                      .start = (frame.size().height/2) - 1, .stop = 3*(frame.size().height/4) + 1};

    threadStruct thread3 = {.inputFrame = &frame, .procFrame = &displayFrame, 
                      .rows = frame.size().width, .cols = frame.size().height, 
                      .start = (3*(frame.size().height/4)) -1 , .stop = frame.size().height};
                      

    setupFunction(numThreads);

    int ret0 = pthread_create(&thread[0], NULL, grayscale_sobel,(void *)&thread0);
    int ret1 = pthread_create(&thread[1], NULL, grayscale_sobel,(void *)&thread1);
    int ret2 = pthread_create(&thread[2], NULL, grayscale_sobel,(void *)&thread2);
    int ret3 = pthread_create(&thread[3], NULL, grayscale_sobel,(void *)&thread3);

    while(1){
    pthread_barrier_wait(&barrier);
      //read frame
      cap >> frame;
      
      if(frame.empty())
        break;
      

      imshow("Display image", displayFrame);
      
      if(waitKey(1)==27)
        break; //wait for ESC keystroke in window
    pthread_barrier_wait(&barrier);
    } 

    //close stream
    cap.release();
    destroyAllWindows();

    int ret4 = pthread_join(ret0, NULL);
    int ret5 = pthread_join(ret1, NULL);
    int ret6 = pthread_join(ret2, NULL);
    int ret7 = pthread_join(ret3, NULL);
    
    return 0;
}

void* grayscale_sobel(void* threadArgs){
   
    int Gx, Gy, sum;

    threadStruct *args = (threadStruct *)threadArgs;
    Mat frame_int(args->cols,args->rows,CV_8UC1,Scalar::all(0));

    unsigned char *inputPixels = (unsigned char*) args->inputFrame;
    while(1){
    // Mat B(frame->size().height,frame->size().width,CV_8U);
    //apply greyscale to all pixels
    for(int i = args->start; i<args->stop; i++){
        for(int j = 0; j<args->inputFrame->size().width; j++){
            Vec3b colors = args->inputFrame->at<Vec3b>(Point(j,i));
            unsigned char grey = colors.val[0]*0.0722 + colors.val[1]*0.7152 + colors.val[2]*0.2126;
           // args->procFrame->at<unsigned char>(Point(j,i)) = grey;
            frame_int.at<unsigned char>(Point(j,i)) = grey;
        }

    }
    
    for(int i = 1; i < args->inputFrame->size().width - 1; i++){ //change start and stop
        for(int j = 1 + args->start; j < args->stop - 1; j++){
            Gx = frame_int.at<unsigned char>(Point(i+1,j-1)) + frame_int.at<unsigned char>(Point(i+1,j+1)) - 
                frame_int.at<unsigned char>(Point(i-1,j-1)) - frame_int.at<unsigned char>(Point(i-1,j+1)) +
                2*(frame_int.at<unsigned char>(Point(i+1,j)) - frame_int.at<unsigned char>(Point(i-1,j)));
            
            Gy = frame_int.at<unsigned char>(Point(i-1,j-1)) + frame_int.at<unsigned char>(Point(i+1,j-1)) - 
                frame_int.at<unsigned char>(Point(i-1,j+1)) - frame_int.at<unsigned char>(Point(i+1,j+1)) +
                2*(frame_int.at<unsigned char>(Point(i,j-1)) - frame_int.at<unsigned char>(Point(i,j+1)));
            sum = (abs(Gx) + abs(Gy));
            //cap sum to 8-bit unsigned char
            if(sum > 255){
                sum = 255;
            }    
            args->procFrame->at<unsigned char>(Point(i,j)) = (unsigned char) sum;
        }
    }
    pthread_barrier_wait(&barrier);
    }
   
    return 0;
}