#ifndef CAMERATHREAD_H
#define CAMERATHREAD_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "camera.h"
#include <QObject>
#include <QThread>
#include <time.h>
#include <QImage>

using namespace cv;

class CameraThread : public QThread
{
    Q_OBJECT
private:
    /*Atributos do tempo*/
    bool bStop;
    struct timespec ts;

    /*Classe que manipula a camera*/
    Camera* cam;

    void __msleep(int ms);

protected:
    void run();
public:
    explicit CameraThread(QObject *parent = 0);
    ~CameraThread();

    void stop();

    void play();

    bool isStopped() const;

signals:
    void setFrameCapture(cv::Mat m);
    void frameToQImage(QImage image);
    void getFPSCam(double fps);

};

#endif // CAMERATHREAD_H
