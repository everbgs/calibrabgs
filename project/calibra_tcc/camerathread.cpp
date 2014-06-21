#include "camerathread.h"

CameraThread::CameraThread(QObject *parent) :
    QThread(parent)
{
    this->bStop = true;

    this->cam = new Camera();
   //this->camera->openCamera("http://admin:admin@192.168.1.200/GetData.cgi?CH=2?resolution=800x592&req_fps=30&.mjpg");
    this->cam->openCamera(0);

}

CameraThread::~CameraThread()
{
    this->bStop = true;
    delete this->cam;
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
    Mat frame, oldFrame;
    QImage imagem;

    while (!this->bStop)
    {
        if (!this->cam->readFrame(frame))
        {
            this->bStop = true;
            continue;
        }        

        emit setFrameCapture(frame);

        oldFrame = frame.clone();
        cv::cvtColor(oldFrame, oldFrame, CV_BGR2RGB);

        imagem = QImage((const unsigned char*)oldFrame.data, oldFrame.cols, oldFrame.rows, oldFrame.step, QImage::Format_RGB888);
        emit frameToQImage(imagem);

        this->__msleep(20);
    }    
    cam->stopCamera();
}
