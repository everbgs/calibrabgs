#ifndef CAMERALABEL_H
#define CAMERALABEL_H

#include <QLabel>
#include <QMouseEvent>

class CameraLabel : public QLabel
{
    Q_OBJECT

private:
    int mouseX, mouseY;

public:
    explicit CameraLabel(QWidget *parent = 0);

    void mousePressEvent(QMouseEvent *ev);

signals:
    void onMouseDown(int x, int y);

public slots:

};

#endif // CAMERALABEL_H
