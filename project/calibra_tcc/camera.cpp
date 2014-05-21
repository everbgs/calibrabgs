#include "camera.h"

Camera::Camera()
{
    this->camera = new VideoCapture();
}

Camera::~Camera()
{
    delete this->camera;
}

bool Camera::isCameraOpen(void)
{
    return this->camera->isOpened();
}

void Camera::stopCamera()
{    
    this->camera->release();
}

bool Camera::readFrame(Mat& frame)
{
    return this->camera->read(frame);
}

void Camera::openCamera(int device)
{
    this->camera->open(device);
}

void Camera::openCamera(string device)
{
    this->camera->open(device);
}
