#include "bola.h"

Bola::Bola()
{
    this->coordX = this->coordY = this->angulo = 0;
}

bool Bola::coordenadasObjeto(Mat &frame)
{    
    if (!this->isExistsColor(cores::LARANJA))
        return false;

    cor = this->getColor(cores::LARANJA)._cores;
    //qDebug() << "MAX, R: " << cor[0] << " G: " << cor[1] << " B: " << cor[2];
    //qDebug() << "MIN, R: " << cor[3] << " G: " << cor[4] << " B: " << cor[5];

    //MAX: 0..2, MIN: 3..5
    inRange(frame, Scalar(cor[5], cor[4], cor[3]), Scalar(cor[2], cor[1], cor[0] ), this->gray);
    GaussianBlur(this->gray, this->gray, cv::Size(9,9), 2,2);

    HoughCircles(this->gray, circulos, CV_HOUGH_GRADIENT, 2, this->gray.rows/8, 200, 100);

    if (circulos.size() > 0)
    {
        this->coordX = cvRound(circulos[0][0]);
        this->coordY = cvRound(circulos[0][1]);

        return true;
    }
    return false;
}
