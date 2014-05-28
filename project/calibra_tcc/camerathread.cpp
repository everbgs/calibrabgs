#include "camerathread.h"

CameraThread::CameraThread(QObject *parent) :
    QThread(parent)
{
    this->bStop = true;
}

void CameraThread::setCamera(Camera *c)
{
    this->cam = c;
}

void CameraThread::__msleep(int ms)
{
    this->ts.tv_sec = ms / 1000;
    this->ts.tv_nsec = (ms % 1000) * 1000 * 1000;
    nanosleep(&ts, NULL);
}


void CameraThread::stop() { this->bStop = true; }

void CameraThread::play()
{
    if (!isRunning())
    {
        if (this->isStopped())
            this->bStop = false;
        //Inicia o run(), se estiver em execução não faz nada
        start();
    }
}

bool CameraThread::isStopped() const { return this->bStop; }

void CameraThread::run()
{
    Mat frame;
    QImage imagem;

    while (!this->bStop)
    {
        if (!this->cam->readFrame(frame))
        {
            this->bStop = true;
            continue;
        }        

        emit setFrameCapture(frame);

        cv::cvtColor(frame, frame, CV_BGR2RGB);

        imagem = QImage((const unsigned char*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
        emit frameToQImage(imagem);

        this->__msleep(20);
    }
}
