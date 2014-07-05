#ifndef CAMERA_H
#define CAMERA_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

class Camera
{

private:
    /* Classe responsavel pela captura*/

public:
      VideoCapture* camera;
    Camera();
    ~Camera();

    /*verifica se a camera está aberta*/
    bool isCameraOpen(void);

    /*Para a camera de captura*/
    void stopCamera(void);

    /*Verifica se foi póssivel ler um frame*/
    bool readFrame(Mat& frame);

    /*Abre camera para captura*/
    void openCamera(int device);
    void openCamera(string device);
};

#endif // ROBO_H
