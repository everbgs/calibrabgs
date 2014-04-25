#include "objeto.h"

Objeto::Objeto(){}

Objeto::~Objeto()
{
    this->mapaCores.clear();
}

void Objeto::exportarArquivo(string dir)
{
    if (this->mapaCores.empty()) return;

    ofstream arquivo(dir.c_str(), ios_base::out);

    if (!arquivo.is_open())
        throw "Erro ao exportar arquivo para gravacao";

    map<string, _corcalibra>::iterator it;
    _corcalibra cAux;

    for (it = this->mapaCores.begin(); it != mapaCores.end(); it++)
    {
        cAux = it->second;
        arquivo << it->first << ";" << cAux.maxR << ";" << cAux.maxG << ";" << cAux.maxB
                             << ";" << cAux.minR << ";" << cAux.minG << ";" << cAux.minB << endl;
    }
    arquivo.close();
}

void Objeto::setColor(string nameCor, _corcalibra range)
{
    this->mapaCores[nameCor] = range;
}

