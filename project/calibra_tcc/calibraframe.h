#ifndef CALIBRAFRAME_H
#define CALIBRAFRAME_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include "camera.h"
#include <QImage>
#include <time.h>
#include <limits.h>
#include <vector>
#include <QDebug>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

class CalibraFrame : public QThread
{
    Q_OBJECT

private:
    Camera* camera;
    bool bStop;
    bool exibe_circulo;
    bool visaoColor;
    struct timespec ts;
    QMutex mutex;
    QWaitCondition condition;

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

    void setExibeCirculo(bool visible);

    bool isVisaoRGB() const;

signals:
    void frameToQImage(QImage image);
    void fpsCapture(double fps);

};

#endif // CALIBRAFRAME_H
