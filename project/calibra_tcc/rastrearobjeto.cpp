#include "rastrearobjeto.h"

RastrearObjeto::RastrearObjeto(Objeto *objetos, QObject *parent) :
    QThread(parent)
{
    this->objetos = objetos;
}

void RastrearObjeto::setObjeto(Objeto* o) { this->objetos = o; }

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
    //localizarBola();
    localizarRobos();
}

void RastrearObjeto::localizarBola(void)
{
    if (!this->objetos->isExistsColor(cores::LARANJA))
        return;

    vector<Vec3f> circulos;
    Mat gray;
    int* cor = this->objetos->getColor(cores::LARANJA)._cores;
    qDebug() << "MAX, R: " << cor[0] << " G: " << cor[1] << " B: " << cor[2];
    qDebug() << "MIN, R: " << cor[3] << " G: " << cor[4] << " B: " << cor[5];

    //MAX: 0..2, MIN: 3..5
    inRange(this->matriz, Scalar(cor[5], cor[4], cor[3]), Scalar(cor[2], cor[1], cor[0] ), gray);
    GaussianBlur(gray, gray, cv::Size(9,9), 2,2);


    HoughCircles(gray, circulos, CV_HOUGH_GRADIENT, 2, gray.rows/8, 200, 100);
    if (circulos.size() > 0)
    {
        qDebug() << "Circulo";
        emit getObjCoordenadas(cvRound(circulos[0][0]), cvRound(circulos[0][1]));
    }
}

void RastrearObjeto::localizarRobos()
{

    if ((!this->objetos->isExistsColor(cores::AZUL))||
        (!this->objetos->isExistsColor(cores::VERDE)))
        return;


    vector<Vec3f> circulos1;
    Mat gray1, gray2;
    int* cor1 = this->objetos->getColor(cores::AZUL)._cores;
    int* cor2 = this->objetos->getColor(cores::VERDE)._cores;

    //MAX: 0..2, MIN: 3..5
    inRange(this->matriz, Scalar(cor1[5], cor1[4], cor1[3]), Scalar(cor1[2], cor1[1], cor1[0] ), gray1);
    inRange(this->matriz, Scalar(cor2[5], cor2[4], cor2[3]), Scalar(cor2[2], cor2[1], cor2[0] ), gray2);

    GaussianBlur(gray1, gray1, cv::Size(9,9), 2,2);
    GaussianBlur(gray2, gray2, cv::Size(9,9), 2,2);

    HoughCircles(gray1, circulos1, CV_HOUGH_GRADIENT, 2, gray1.rows/8, 200, 100);

    int x1, x2, y1, y2, a, b;
    x1 = x2 = y1 = y2 = 0;
    if (circulos1.size() > 0)
    {
        x1 = cvRound(circulos1[0][0]);
        y1 = cvRound(circulos1[0][1]);
    }
    else
        return;

    circulos1.clear();
    HoughCircles(gray2, circulos1, CV_HOUGH_GRADIENT, 2, gray2.rows/8, 200, 100);
    if (circulos1.size() > 0)
    {        
        x2 = cvRound(circulos1[0][0]);
        y2 = cvRound(circulos1[0][1]);
    }    
    if (x2 || y2)
    {
        a = x1 - x2;
        b = y1 - y2;
        double angulo = atan2(b, a) * 180 / RastrearObjeto::PI;
        qDebug() << a << " " << b;
        emit getObjCoordenadas(x1, y1, angulo);
    }
    else
        emit getObjCoordenadas(x1, y1);

    qDebug() << "X1: " << x1 << " Y1: " << y1 << " X2: " << x2 << " Y2: " << y2;
}
