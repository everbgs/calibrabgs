#ifndef CAMERATHREAD_H
#define CAMERATHREAD_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "camera.h"
#include <QObject>
#include <QThread>
#include <time.h>
#include <QImage>
#include <QMutex>
#include "typesapp.h"

using namespace cv;

class CameraThread : public QThread
{
    Q_OBJECT
private:
    /*Atributos do tempo*/
    struct timespec ts;

    /*Classe que manipula a camera*/
    Camera* cam;

    bool bStop;

    QMutex mutex;

    void __msleep(int ms);

protected:
    void run();
public:
    explicit CameraThread(QObject *parent = 0);

    void stop();

    void play();

    bool isStopped() const;

    void setCamera(Camera* c);

signals:
    void setFrameCapture(cv::Mat m);
    void frameToQImage(QImage image);
    void statusMethodThread(ThreadType m);
};

#endif // CAMERATHREAD_H
