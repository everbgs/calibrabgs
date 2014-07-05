#include <QApplication>
#include "principal.h"

Q_DECLARE_METATYPE(cv::Mat)
Q_DECLARE_METATYPE(ThreadType)

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Principal w;

    qRegisterMetaType<cv::Mat>("cv::Mat");
    qRegisterMetaType<ThreadType>("ThreadType");


    w.show();
    
    return a.exec();
}
