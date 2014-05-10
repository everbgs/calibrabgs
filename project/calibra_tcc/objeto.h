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

//Estrutura para guardar valores Max e Min das cores
struct _corcalibra {    
    //MAX: 0..2, MIN: 3..5
    int _cores[6];
};

class Objeto
{

private:
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
