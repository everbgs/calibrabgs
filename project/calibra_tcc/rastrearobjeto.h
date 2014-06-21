#ifndef RASTREAROBJETO_H
#define RASTREAROBJETO_H

#include <QObject>
#include <QThread>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QDebug>
#include <math.h>
#include <vector>
#include "objeto.h"
#define __pow__(a)((a)*(a))


using namespace std;
using namespace cv;

typedef pair<int, int> pii;
typedef vector<pii> vp;

class RastrearObjeto : public QThread
{
    Q_OBJECT
private:
    Objeto* objetos;
    Mat matriz;    
    static const double PI = 3.14159265;
    vp time, outros;

    void appendVectorColor(string color, vp& estru);

    void localizarBola(void);
    void localizarRobos(void);

    int cmp(double x, double y, double EPS = 1e-10);

protected:
    void run();

public:
    explicit RastrearObjeto(Objeto* objetos, QObject *parent = 0);
    void setObjeto(Objeto* o);

public slots:
    void receberFrame(cv::Mat m);
signals:
    void getObjCoordenadas(QString name, int x, int y);
    void getObjCoordenadas(QString name, int x, int y, double ang);

};

#endif // RASTREAROBJETO_H
