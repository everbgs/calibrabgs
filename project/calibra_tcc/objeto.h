#ifndef OBJETO_H
#define OBJETO_H

#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include "typesapp.h"

using namespace std;

class Objeto
{
protected:
    //Mapa das cores, Key = Cor, Value = RGB Max, Min
    map<string, _corcalibra> mapaCores;
public:
    Objeto();
    ~Objeto();

    //Importação e Exportação das cores
    void exportarArquivo(string dir);
    void importarArquivo(string dir);

    //Adiciona a Cor juntamente com seus intervalos
    void setColor(string nameCor, _corcalibra range);

    //Devolve a cor Informada
    _corcalibra getColor(string nameCor);

    //Se Existe a cor
    bool isExistsColor(string cor);

};

#endif // OBJETO_H
