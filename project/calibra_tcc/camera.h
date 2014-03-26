#ifndef CAMERA_H
#define CAMERA_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


using namespace cv;

class Camera
{

private:
    VideoCapture camera;
    Mat frameAtual;

public:
    bool isCameraOpen(void);

    Mat nextFrame(void);

    void openCamera(int device);
};

#endif // ROBO_H
