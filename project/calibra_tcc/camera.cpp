#include "camera.h"


bool Camera::isCameraOpen(void)
{
    return this->camera.isOpened();
}

Mat Camera::nextFrame(void)
{
    Mat frame;
    this->camera >> frame;
    return frame;
}

void Camera::openCamera(int device)
{
    this->camera.open(device);
}
