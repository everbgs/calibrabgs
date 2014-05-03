#ifndef OBJETO_H
#define OBJETO_H

#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>

using namespace std;

namespace cores {
    const string AZUL = "AZUL";
    const string AMARELO = "AMARELO";
    const string VERDE = "VERDE";
    const string ROSA = "ROSA";
    const string LARANJA = "LARANJA";
}

struct _corcalibra {
    int _cores[6]; //MAX: 0..2, MIN: 3..5
};

class Objeto
{

private:
    map<string, _corcalibra> mapaCores;
public:
    Objeto();

    ~Objeto();

    void exportarArquivo(string dir);
    void importarArquivo(string dir);

    void setColor(string nameCor, _corcalibra range);
    _corcalibra getColor(string nameCor);

    bool isColor(string cor);

};

#endif // OBJETO_H
