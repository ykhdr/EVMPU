#include <opencv2/opencv.hpp>
#include <ctime>
#include <iostream>
#include <chrono>

using namespace std;
using namespace cv;
using namespace std::chrono;

int main(int, char**) {
    
    VideoCapture camera(0);
    namedWindow("Webcum");

    // this will contain the image from the webcam
    Mat frame;
    long frameCount = 0;
    long double totalTime = 0, inputTime = 0, procTime = 0, outputTime = 0;
    long double differenceFrameTime = 0, sum = 0;
    long double prevFrameTime = 0, newFrameTime = 0;

    bool isBreak = false;
    
    long double fps = 0;


    


    // display the frame until you press a key
    while (1) {

        ++frameCount;
        
        auto start = steady_clock::now();

        for (size_t i = 0; i < 60; i++)
        {
            clock_t c_start = clock();
            
            camera >> frame;

            inputTime += (clock() - c_start);
            c_start = clock();

            // add blur on frame
            // GaussianBlur(frame, frame, Size(0, 0), 3);

            putText(frame, "FPS: " + to_string(fps), Point(10, 70), FONT_ITALIC, 2, Scalar(0, 255, 255));

            procTime += (clock() - c_start);

            c_start = clock();

            imshow("Webcum", frame);

            if (waitKey(1) >= 0)
                isBreak = true;

            outputTime += (clock() - c_start);

        }

        auto end = steady_clock::now();
        duration<double> sec = duration_cast<duration<double>>(end - start);
        fps = (double)60 / sec.count();
        sum += fps;

        if (isBreak)
        {
            break;
        }
        
    }
    cout << clock() << std::endl;
    cout << "frame per second: " << sum/(frameCount) << endl;
    totalTime = inputTime + procTime + outputTime;
    double procent = totalTime / 100.0;
    cout << "Reading time: " << inputTime / procent << "%" << endl;
    cout << "Processing time : " << procTime / procent << "%" << endl;
    cout << "The time of the withdrawal: " << outputTime / procent << "%" << endl;
    camera.open(0);
    return 0;

}