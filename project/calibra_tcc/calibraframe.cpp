#include "calibraframe.h"

CalibraFrame::CalibraFrame(QObject *parent) : QThread(parent)
{
    this->visaoColor = true;
    this->bStop = true;
    this->exibe_circulo = false;

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

    QImage imagem;
    Mat frame, dst, dst2;
    vector<Vec3f> cir;
    Point center;
    int raio;
    unsigned int i;

    int cnt = 0;
    time_t start, end;
    double sec, fps;

    time(&start);

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

        cv::inRange(frame, cv::Scalar(128, 4, 0), cv::Scalar(233, 148, 75), dst);
        cv::GaussianBlur(dst,dst,cv::Size(9,9), 2,2);

        //cv::inRange(frame, cv::Scalar(20, 135, 177), cv::Scalar(0, 27, 94), dst2);
        cv::inRange(frame, cv::Scalar(0, 27, 94), cv::Scalar(20, 135, 117), dst2);
        cv::GaussianBlur(dst2,dst2,cv::Size(9,9), 2,2);

        if (this->visaoColor)
        {
            if (this->exibe_circulo)
            {
                cv::HoughCircles(dst2, cir, CV_HOUGH_GRADIENT, 2, dst2.rows/4,200, 100);

                for(i = 0; i < cir.size(); i++ )
                {
                    center.x = cvRound(cir[i][0]);
                    center.y = cvRound(cir[i][1]);
                    raio = cvRound(cir[i][2]);
                    qDebug() << "Bola 1 X: " << center.x << " Y: " << center.y;
                    circle(frame, center, raio, Scalar(0,0,255), 2, CV_AA);
                }

                cv::HoughCircles(dst, cir, CV_HOUGH_GRADIENT, 2, dst.rows/4,200, 100);

                for(i = 0; i < cir.size(); i++ )
                {
                    center.x = cvRound(cir[i][0]);
                    center.y = cvRound(cir[i][1]);
                    raio = cvRound(cir[i][2]);
                    qDebug() << "Bola 2 X: " << center.x << " Y: " << center.y;
                    circle(frame, center, raio, Scalar(0,0,255), 2, CV_AA);
                }



            }
            cv::cvtColor(frame, frame, CV_BGR2RGB);
            imagem = QImage((const unsigned char*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
        }
        else        
        {

            imagem = QImage(dst2.data, dst2.cols, dst2.rows, dst2.step, QImage::Format_Indexed8);
        }


        //FPS
        time(&end);
        if (cnt == INT_MAX) cnt = 0;
        ++cnt;
        sec = difftime(end,start);
        fps = cnt/sec;

        emit fpsCapture(fps);
        emit frameToQImage(imagem);
        this->msleep(20);
    }
    this->camera->stopCamera();
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

bool CalibraFrame::isStopped() const { return this->bStop; }

void CalibraFrame::setVisaoRGB(bool enabled){ this->visaoColor = enabled; }

void CalibraFrame::setExibeCirculo(bool visible){ this->exibe_circulo = visible; }

bool CalibraFrame::isVisaoRGB() const { return this->visaoColor; }
