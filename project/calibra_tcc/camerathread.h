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
    bool bStop;
    struct timespec ts;
    Camera* cam;


    void __msleep(int ms);

protected:
    void run();
public:
    explicit CameraThread(QObject *parent = 0);

    void setCamera(Camera* c);

    void stop();

    void play();

    bool isStopped() const;

signals:
    void setFrameCapture(cv::Mat m);
    void frameToQImage(QImage image);

};

#endif // CAMERATHREAD_H
