#ifndef OBJETORASTREAVEL_H
#define OBJETORASTREAVEL_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;


class ObjetoRastreavel
{

public:
    int coordX, coordY, angulo;
    ObjetoRastreavel();

    virtual bool coordenadasObjeto(Mat&);
};

#endif // OBJETORASTREAVEL_H
