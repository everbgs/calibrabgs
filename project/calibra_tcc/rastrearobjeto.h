#ifndef RASTREAROBJETO_H
#define RASTREAROBJETO_H

#include <QObject>
#include <QThread>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "objetorastreavel.h"
#include <QDebug>

using namespace std;

using namespace cv;

class RastrearObjeto : public QThread
{
    Q_OBJECT
private:
    ObjetoRastreavel* obj;   
    Mat matriz;

protected:
    void run();

public:
    explicit RastrearObjeto(QObject *parent = 0);
    void setObjeto(ObjetoRastreavel* o);    

public slots:
    void receberFrame(cv::Mat m);
signals:
    void getObjCoordenadas(int x, int y);
};

#endif // RASTREAROBJETO_H
