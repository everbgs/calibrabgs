#include "calibraframe.h"

CalibraFrame::CalibraFrame(QObject *parent) : QThread(parent)
{
    this->visaoColor = true;
    this->bStop = true;

    this->camera = new Camera();
    this->camera->openCamera(0);

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
    delete this->camera;    
}

void CalibraFrame::run()
{  
    if (!this->camera->isCameraOpen())
        return;

    Mat frame, dst;
    while (!this->bStop)
    {
        {
            QMutexLocker locker(&this->mutex);
            if (this->bStop) continue;
        }

        if (!this->camera->readFrame(frame))
        {            
            this->bStop = true;
            continue;
        }

        if (this->visaoColor)
        {
            cv::cvtColor(frame, frame, CV_BGR2RGB);
            this->imagem = QImage((const unsigned char*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
        }
        else
        {
            /*Max = claro, min = escuro*/
            cv::inRange(frame, cv::Scalar(BMin, GMin, RMin), cv::Scalar(BMax, GMax, RMax), dst);
            cv::GaussianBlur(dst,dst,cv::Size(9,9), 2,2);

            this->imagem = QImage(dst.data, dst.cols, dst.rows, dst.step, QImage::Format_Indexed8);
        }        
        emit frameToQImage(this->imagem);
        this->msleep(20);
    }
}

void CalibraFrame::stop()
{  
    QMutexLocker locker(&this->mutex);
    this->bStop = true;
}

void CalibraFrame::play()
{
    if (!isRunning())
    {         
        if (this->isStopped())
            this->bStop = false;
        //Inicia o run(), se estiver em execução não faz nada
        start();
    }
}

void CalibraFrame::msleep(int ms)
{
    this->ts.tv_sec = ms / 1000;
    this->ts.tv_nsec = (ms % 1000) * 1000 * 1000;
    nanosleep(&ts, NULL);
}

bool CalibraFrame::isStopped() const
{
    return this->bStop;
}

void CalibraFrame::setVisaoRGB(bool enabled)
{
    this->visaoColor = enabled;
}

bool CalibraFrame::isVisaoRGB() const
{
    return this->visaoColor;
}
