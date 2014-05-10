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
    int i;

    //Exporta para cada cor no formato: COR;RMax;GMax;BMax;RMin;GMin;BMin
    for (it = this->mapaCores.begin(); it != mapaCores.end(); it++)
    {
        cAux = it->second;
        arquivo << it->first;
        for (i = 0; i<6; i++)
            arquivo << ";" << cAux._cores[i];
        arquivo << endl;

    }
    arquivo.close();
}

void Objeto::importarArquivo(string dir)
{
    ifstream arquivo(dir.c_str());

    if (!arquivo.is_open() || !arquivo.good())
        throw "Erro ao importar arquivo";

    mapaCores.clear();

    string str, token, cor;
    _corcalibra c;
    int cont, ix;

    //Importa para cada cor no formato: COR;RMax;GMax;BMax;RMin;GMin;BMin
    while (!arquivo.fail())
    {
        cont = 1;
        ix = 0;
        arquivo >> str;
        istringstream ss(str);

        while(getline(ss, token, ';'))
        {
            if (cont == 1) //Cor
                cor = token;
            else if (cont < 8) //RGB MAX, MIN
                c._cores[ix++] = strtol(token.c_str(), NULL, 10);

            ++cont;
        }
        //Salva a cor importada juntamente com seus intervalos
        mapaCores[cor] = c;
    }
    arquivo.close();
}

void Objeto::setColor(string nameCor, _corcalibra range)
{
    this->mapaCores[nameCor] = range;
}

_corcalibra Objeto::getColor(string nameCor)
{
    return this->mapaCores[nameCor];
}

bool Objeto::isExistsColor(string cor)
{
    return (this->mapaCores.count(cor) != 0);
}

