#ifndef CALIBRAFRAME_H
#define CALIBRAFRAME_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include "camera.h"
#include <QImage>
#include <time.h>
#include <QDebug>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

class CalibraFrame : public QThread
{
    Q_OBJECT

private:
    Camera* camera;
    bool bStop;
    bool visaoColor;
    struct timespec ts;
    QMutex mutex;
    QWaitCondition condition;
    QImage imagem;

protected:
    void run();

public:
    explicit CalibraFrame(QObject *parent = 0);
    ~CalibraFrame();

    int RMax, RMin;
    int GMax, GMin;
    int BMax, BMin;

    void stop();

    void play();

    void msleep(int ms);

    bool isStopped() const;

    void setVisaoRGB(bool enabled);

    bool isVisaoRGB() const;

signals:
    void frameToQImage(QImage image);
};

#endif // CALIBRAFRAME_H
