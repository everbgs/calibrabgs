#ifndef CALIBRAFRAME_H
#define CALIBRAFRAME_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include "camera.h"
#include <QImage>
#include <time.h>
#include <vector>
#include "typesapp.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

class CalibraFrame : public QThread
{
    Q_OBJECT
private:
    /*Classe que manipula a camera*/
    Camera* camera;

    /*Atributos utilizados pela classe*/
    bool bStop;
    bool exibe_circulo;
    bool visaoColor;


    struct timespec ts;
    QMutex mutex;
protected:
    /*Quando executar automaticamente executa este método*/
    void run();

public:
    explicit CalibraFrame(QObject *parent = 0);
    ~CalibraFrame();

    /*limites para utilizar o thresholding*/
    int RMax, RMin;
    int GMax, GMin;
    int BMax, BMin;

    /*Pausa a captura de frames*/
    void stop();

    /*Inicia a captura de frames*/
    void play();

    /*Dalay*/
    void msleep(int ms);

    /*Retorna a situação da thread*/
    bool isStopped() const;

    /*Retorna a situação da thread*/
    void setVisaoRGB(bool enabled);

    void setExibeCirculo(bool visible);

    bool isVisaoRGB() const;

    void setCamera(Camera* cam);

signals:
    /*Sinal emitido para exibir os frames nos,
     *componentes do QT*/
    void frameToQImage(QImage image);    
    void statusMethodThread(ThreadType m);
};

#endif // CALIBRAFRAME_H
