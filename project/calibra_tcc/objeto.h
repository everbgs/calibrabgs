#ifndef OBJETO_H
#define OBJETO_H

#include <iostream>
#include <map>

using namespace std;

struct Cor {
    int red,green, blue;
    Cor(int r, int g, int b): red(r), green(g), blue(b){}
};


class Objeto
{

private:
    map<string, Cor> mapaCores;
public:
    Objeto();
    ~Objeto();

    string idName;
    void exportarArquivo(void);
    void importarArquivo(void);

    void setColor(string cor, int r, int b, int g);
    void setColor(string cor, Cor rgb);
};

#endif // OBJETO_H
