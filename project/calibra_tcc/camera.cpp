#include "camera.h"


bool Camera::isCameraOpen(void)
{
    return this->camera.isOpened();
}

Mat Camera::nextFrame(void)
{
    this->camera >> this->frameAtual;
    return this->frameAtual;
}

void Camera::openCamera(int device)
{
    this->camera.open(device);
}
