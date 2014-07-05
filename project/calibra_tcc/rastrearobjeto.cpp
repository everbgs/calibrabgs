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
    /* Se existe a cor Laranja então continua */
    if (!this->objetos->isExistsColor(cores::LARANJA))
        return;

    vector<Vec3f> circulos;
    Mat gray;

    /* Retorna o vetor dos limites das cores */
    int* cor = this->objetos->getColor(cores::LARANJA)._cores;

    /*Limite de cores MAX: 0..2, MIN: 3..5*/
    inRange(this->matriz, Scalar(cor[5], cor[4], cor[3]), Scalar(cor[2], cor[1],
                                                                 cor[0] ), gray);

    /*Suaviza a imagem para detectar os círculos*/
    GaussianBlur(gray, gray, cv::Size(9,9), 2,2);

    /*Executa a transformada de Hough*/
    HoughCircles(gray, circulos, CV_HOUGH_GRADIENT, 2, gray.rows/8, 200, 100);

    /*Se achou então emite um sinal das coordenadas*/
    if (circulos.size() > 0)
        emit getObjCoordenadas("Bola: ", cvRound(circulos[0][0]), cvRound(circulos[0][1]));
}

void RastrearObjeto::appendVectorColor(string color, vp& estru)
{
    if (!this->objetos->isExistsColor(color))
        return;

    int *cor, max;
    Mat gray;
    vector<Vec3f> circulos;

    cor = this->objetos->getColor(color)._cores;
    inRange(this->matriz, Scalar(cor[5], cor[4], cor[3]), Scalar(cor[2], cor[1], cor[0]), gray);
    GaussianBlur(gray, gray, cv::Size(9,9), 2,2);
    HoughCircles(gray, circulos, CV_HOUGH_GRADIENT, 2, gray.rows/8, 200, 100);

    max = circulos.size();
    for (int i=0; i<max; i++)
    {
        estru.push_back( make_pair( cvRound(circulos[i][0]),
                                     cvRound(circulos[i][1])
                                   ));
    }    
}

void RastrearObjeto::localizarRobos()
{
    this->time.clear();
    this->outros.clear();

    /*Encontra todos os circulos utilizando a transformada de houg*/
    appendVectorColor(cores::AZUL, this->time);
    appendVectorColor(cores::AMARELO, this->time);
    appendVectorColor(cores::VERDE, this->outros);
    appendVectorColor(cores::ROSA, this->outros);   

    int i, j, mi, mj;
    mi = this->time.size();
    mj = this->outros.size();


    int ix, x1, x2, y1, y2, a, b;
    double euc, men, angulo;

    /*Para as cores que representam os times (Azul, Amarelo) */
    for (i=0; i<mi; i++)
    {
        ix = -1;
        men = 999999;

        x1 = time[i].first;
        y1 = time[i].second;

        /*Para as cores secundarias (Rosa, Verde) */
        for (j=0; j<mj; j++)
        {
            /*Executa a distancia euclidiana para saber o circulo
              mais proximo da etiqueta do robô */
            euc = (double) (__pow__(x1-outros[j].first) + __pow__(y1-outros[j].second));
            euc = sqrt(euc);

            /*Se a distancia for menor que 65 e for menor que a atual menor*/
            if ((euc < 65) && (cmp(euc, men) == -1))
            {
                men = euc;
                ix = j;
            }
        }

        /*Se achou então executa a equação de
         Gonçalvez para achar a direção*/
        if (ix != -1)
        {
            x2 = outros[ix].first;
            y2 = outros[ix].second;
            a = x1 - x2;
            b = y1 - y2;
            angulo = atan2(b, a)* 180 / PI;
            emit getObjCoordenadas("Robo "+QString::number(i+1)+": ", x1, y1, angulo);
        }
        else /*Se não achou apenas retorna as coordenadas do circulo central*/
            emit getObjCoordenadas("Robo "+QString::number(i+1)+": ", x1, y1);
    }
}
