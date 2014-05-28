#include <QApplication>
#include "principal.h"

Q_DECLARE_METATYPE(cv::Mat)

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Principal w;

    qRegisterMetaType<cv::Mat>("cv::Mat");

    w.show();
    
    return a.exec();
}
