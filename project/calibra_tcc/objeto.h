#ifndef OBJETO_H
#define OBJETO_H

#include <iostream>
#include <map>
#include <fstream>
#include <string>

using namespace std;

namespace cores {
    const string AZUL = "AZUL";
    const string AMARELO = "AMARELO";
    const string VERDE = "VERDE";
    const string ROSA = "ROSA";
    const string LARANJA = "LARANJA";
}

struct _corcalibra {
    int maxR, maxG, maxB;
    int minR, minG, minB;
};

class Objeto
{

private:
    map<string, _corcalibra> mapaCores;

    ofstream arquivo;
    string nameArq;
    string caminho;
public:
    Objeto();

    ~Objeto();

    void exportarArquivo(string dir);
    void importarArquivo(string dir);

    void setColor(string nameCor, _corcalibra range);
};

#endif // OBJETO_H
