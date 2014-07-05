#include "calibraframe.h"

CalibraFrame::CalibraFrame(QObject *parent) : QThread(parent)
{
    this->visaoColor = true;
    this->bStop = true;
    this->exibe_circulo = false;

    this->RMax = 0;
    this->RMin = 0;
    this->GMax = 0;
    this->GMin = 0;
    this->BMax = 0;
    this->BMin = 0;
}

CalibraFrame::~CalibraFrame()
{
    this->bStop = true;
}

void CalibraFrame::run()
{  
    QImage imagem;
    Mat frame, dst;
    vector<Vec3f> cir;
    Point center;
    int raio;
    unsigned int i;

    while (!this->bStop)
    {
        if (!this->camera->readFrame(frame))
        {
            this->bStop = true;
            continue;
        }

        /*Threshold para encontrar a região de interesse*/
        cv::inRange(frame, cv::Scalar(BMin, GMin, RMin), cv::Scalar(BMax, GMax, RMax),
                                                                                 dst);

        /*Suavização da imagem para processamentos posteriores */
        cv::GaussianBlur(dst, dst, cv::Size(9,9), 2,2);

        /*Se é para mostrar os frames no formato RGB*/
        if (this->visaoColor)
        {

            /*Se foi marcado para utilizar a TH*/
            if (this->exibe_circulo)
            {
                /*Aplica a TH sobre a região de interrese */
                cv::HoughCircles(dst, cir, CV_HOUGH_GRADIENT, 2, dst.rows/8, 200, 100);

                /*Se retornar algum circulo então, desenha sobre o frame atual os mesmos*/
                for(i = 0; i < cir.size(); i++ )
                {
                    center.x = cvRound(cir[i][0]);
                    center.y = cvRound(cir[i][1]);
                    raio = cvRound(cir[i][2]);                   
                    circle(frame, center, raio, Scalar(0,0,255), 2, CV_AA);
                    //qDebug() << "X: " << center.x << " Y: " << center.y << " R: " << raio;
                }
            }
            /*converte a cor para se adequar a estrutura QT*/
            cv::cvtColor(frame, frame, CV_BGR2RGB);
            imagem = QImage((const unsigned char*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
        }
        else /*Mostra somente a região de interesse da calibração */
            imagem = QImage(dst.data, dst.cols, dst.rows, dst.step, QImage::Format_Indexed8);

        /*Exibe o frame na tela principal*/
        emit frameToQImage(imagem);

        /*Espera 20 ms */
        this->msleep(20);
    }
    emit statusMethodThread(CAL_FINISH);
}

void CalibraFrame::stop()
{  
    mutex.lock();
    this->bStop = true;
    mutex.unlock();
}

void CalibraFrame::play()
{
    if (!isRunning())
    {         
        if (this->isStopped())
            this->bStop = false;

        emit statusMethodThread(CAL_FIRST);
        start();
    }
}

void CalibraFrame::msleep(int ms)
{
    this->ts.tv_sec = ms / 1000;
    this->ts.tv_nsec = (ms % 1000) * 1000 * 1000;
    nanosleep(&ts, NULL);    
}

bool CalibraFrame::isStopped() const { return this->bStop; }

void CalibraFrame::setVisaoRGB(bool enabled){ this->visaoColor = enabled; }

void CalibraFrame::setExibeCirculo(bool visible){ this->exibe_circulo = visible; }

bool CalibraFrame::isVisaoRGB() const { return this->visaoColor; }

void CalibraFrame::setCamera(Camera *cam)
{
    this->camera = cam;
}
