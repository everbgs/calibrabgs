#include "rastrearobjeto.h"

RastrearObjeto::RastrearObjeto(QObject *parent) :
    QThread(parent)
{
}

void RastrearObjeto::setObjeto(ObjetoRastreavel* o) { this->obj = o; }

void RastrearObjeto::receberFrame(cv::Mat m)
{
    if (!isRunning())
    {
        this->matriz = m;
        start();
    }
}

void RastrearObjeto::run()
{   
    if (obj->coordenadasObjeto(matriz)){
        qDebug() << "X: " << obj->coordX << " Y: " << obj->coordY;
        emit getObjCoordenadas(obj->coordX, obj->coordY);
    }
}
