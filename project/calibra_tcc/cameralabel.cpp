#include "cameralabel.h"
#include <QMessageBox>

CameraLabel::CameraLabel(QWidget *parent) :
    QLabel(parent),
    mouseX(0),
    mouseY(0)
{
}

void CameraLabel::mousePressEvent(QMouseEvent *ev)
{

    this->mouseX = ev->x();
    this->mouseY = ev->y();

    emit onMouseDown(this->mouseX, this->mouseY);
}
