#include "camerathread.h"

CameraThread::CameraThread(QObject *parent) :
    QThread(parent)
{
    this->bStop = true;    
}

void CameraThread::__msleep(int ms)
{
    this->ts.tv_sec = ms / 1000;
    this->ts.tv_nsec = (ms % 1000) * 1000 * 1000;
    nanosleep(&ts, NULL);
}

void CameraThread::stop()
{
    this->mutex.lock();
    this->bStop = true;
    this->mutex.unlock();
}

void CameraThread::play()
{
    if (!isRunning())
    {
        if (this->isStopped())
            this->bStop = false;

        emit statusMethodThread(CAL_FIRST);
        start();
    }
}

bool CameraThread::isStopped() const { return this->bStop; }

void CameraThread::setCamera(Camera *c)
{
    this->cam = c;
}

void CameraThread::run()
{
    Mat frame, oldFrame;
    QImage imagem;

    this->cam->openCamera(__device);

    while (!this->bStop)
    {
        if (!this->cam->readFrame(frame))
        {
            this->bStop = true;
            continue;
        }        

        emit setFrameCapture(frame);

        cv::cvtColor(frame, oldFrame, CV_BGR2RGB);
        imagem = QImage((const unsigned char*)oldFrame.data, oldFrame.cols, oldFrame.rows, oldFrame.step, QImage::Format_RGB888);
        emit frameToQImage(imagem);

        this->__msleep(20);
    }    
     emit statusMethodThread(CAL_FINISH);
}
