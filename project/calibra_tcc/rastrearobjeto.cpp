#include "rastrearobjeto.h"

RastrearObjeto::RastrearObjeto(Objeto *objetos, QObject *parent) :
    QThread(parent)
{
    this->objetos = objetos;
}

int RastrearObjeto::cmp(double x, double y, double EPS)
{
    return ((x <= y + EPS) ? (x + EPS < y) ? -1 : 0 : 1);
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
            angulo = atan2(b, a)* 180 / RastrearObjeto::PI;
            emit getObjCoordenadas("Robo "+QString::number(i+1)+": ", x1, y1, angulo);
        }
        else /*Se não achou apenas retorna as coordenadas do circulo central*/
            emit getObjCoordenadas("Robo "+QString::number(i+1)+": ", x1, y1);
    }

    /*
    vector<Vec3f> circulos1;
    Mat gray1, gray2;
    int* cor1 = this->objetos->getColor(cores::AZUL)._cores;
    int* cor2 = this->objetos->getColor(cores::VERDE)._cores;

    //MAX: 0..2, MIN: 3..5
    inRange(this->matriz, Scalar(cor1[5], cor1[4], cor1[3]), Scalar(cor1[2], cor1[1], cor1[0] ), gray1);
    inRange(this->matriz, Scalar(cor2[5], cor2[4], cor2[3]), Scalar(cor2[2], cor2[1], cor2[0] ), gray2);

    GaussianBlur(gray1, gray1, cv::Size(9,9), 2,2);
    GaussianBlur(gray2, gray2, cv::Size(9,9), 2,2);

    HoughCircles(gray1, circulos1, CV_HOUGH_GRADIENT, 2, gray1.rows/8, 200, 100);

    int x1, x2, y1, y2, a, b;
    x1 = x2 = y1 = y2 = 0;
    if (circulos1.size() > 0)
    {
        x1 = cvRound(circulos1[0][0]);
        y1 = cvRound(circulos1[0][1]);
    }
    else
        return;

    circulos1.clear();
    HoughCircles(gray2, circulos1, CV_HOUGH_GRADIENT, 2, gray2.rows/8, 200, 100);
    if (circulos1.size() > 0)
    {        
        x2 = cvRound(circulos1[0][0]);
        y2 = cvRound(circulos1[0][1]);
    }    
    if (x2 || y2)
    {
        a = x1 - x2;
        b = y1 - y2;
        double angulo = atan2(b, a) * 180 / RastrearObjeto::PI;
        qDebug() << a << " " << b;
        emit getObjCoordenadas("Robo: ", x1, y1, angulo);
    }
    else
        emit getObjCoordenadas("Robo: ", x1, y1);

    qDebug() << "X1: " << x1 << " Y1: " << y1 << " X2: " << x2 << " Y2: " << y2; */
}
