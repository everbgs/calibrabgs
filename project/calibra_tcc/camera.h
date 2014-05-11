#ifndef CAMERA_H
#define CAMERA_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

class Camera
{

private:
    VideoCapture* camera;
public:
    Camera();
    ~Camera();

    bool isCameraOpen(void);
    bool readFrame(Mat& frame);
    void openCamera(int device);
    void openCamera(string device);
};

#endif // ROBO_H
