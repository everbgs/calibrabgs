#ifndef BOLA_H
#define BOLA_H

#include "objeto.h"
#include "objetorastreavel.h"
#include <QDebug>

class Bola : public Objeto, public ObjetoRastreavel
{
private:
    Mat gray;
    int* cor;
    vector<Vec3f> circulos;

public:
    Bola();
    bool coordenadasObjeto(Mat& frame);
};

#endif // BOLA_H
