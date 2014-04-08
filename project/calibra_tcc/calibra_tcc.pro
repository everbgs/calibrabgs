#-------------------------------------------------
#
# Project created by QtCreator 2014-03-21T01:20:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = calibra_tcc
TEMPLATE = app


SOURCES += main.cpp\
        principal.cpp \
    camera.cpp \
    cameralabel.cpp

HEADERS  += principal.h \
    camera.h \
    cameralabel.h

FORMS    += principal.ui

win32 {
    #Relativo a instalação

    INCLUDEPATH += C:\opencv\build\include

    LIBS += C:\opencv\release\install\x64\mingw\bin\libopencv_calib3d248.dll
    LIBS += C:\opencv\release\install\x64\mingw\bin\libopencv_contrib248.dll
    LIBS += C:\opencv\release\install\x64\mingw\bin\libopencv_core248.dll
    LIBS += C:\opencv\release\install\x64\mingw\bin\libopencv_highgui248.dll
    LIBS += C:\opencv\release\install\x64\mingw\bin\libopencv_imgproc248.dll
}

unix {
    INCLUDEPATH += `pkg-config --cflags opencv`
    LIBS += `pkg-config --libs opencv`
}



