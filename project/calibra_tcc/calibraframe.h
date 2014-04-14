#ifndef CALIBRAFRAME_H
#define CALIBRAFRAME_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include "camera.h"
#include <QImage>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class CalibraFrame : public QThread
{
    Q_OBJECT

private:
    Camera* webCam;
    bool bStop;
    QMutex mutexCam;

protected:
    void run();

public:
    explicit CalibraFrame(QObject *parent = 0);
    ~CalibraFrame();
    bool visao;

public slots:
    void stop();
signals:
    void frameToQImage(QImage image);
};

#endif // CALIBRAFRAME_H
