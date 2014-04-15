#include "calibraframe.h"

CalibraFrame::CalibraFrame(QObject *parent) :
    QThread(parent),
    webCam(NULL)
{
    this->visao = true;
    this->bStop = false;

    this->RMax = 0;
    this->RMin = 0;
    this->GMax = 0;
    this->GMin = 0;
    this->BMax = 0;
    this->BMin = 0;
}

CalibraFrame::~CalibraFrame()
{
    delete this->webCam;
}

void CalibraFrame::run()
{
    this->webCam = new Camera();

    this->webCam->openCamera(0);
    if (!this->webCam->isCameraOpen())
        return;

    cv::Mat dst;

    while (true)
    {
        {
            QMutexLocker locker(&mutexCam);
            if (this->bStop) break;
        }

        cv::Mat frame = this->webCam->nextFrame();
        if (frame.empty())
            return;

        if (this->visao)
        {
            cv::cvtColor(frame, frame, CV_BGR2RGB);
            QImage qimgOriginal( frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
            emit frameToQImage(qimgOriginal);
        }
        else
        {
            /*Max = claro, min = escuro*/
            cv::inRange(frame, cv::Scalar(BMin, GMin, RMin), cv::Scalar(BMax, GMax, RMax), dst);
            cv::GaussianBlur(dst,dst,cv::Size(5,5),1.5);

            QImage qimgProcessed(dst.data, dst.cols, dst.rows, dst.step, QImage::Format_Indexed8);
            emit frameToQImage(qimgProcessed);
        }

        this->msleep(20);
    }

    delete this->webCam;
    this->webCam = NULL;
}

void CalibraFrame::stop()
{
    QMutexLocker locker(&mutexCam);
    this->bStop = true;
}
