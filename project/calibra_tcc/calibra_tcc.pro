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
    cameralabel.cpp \
    calibraframe.cpp \
    objeto.cpp

HEADERS  += principal.h \
    camera.h \
    cameralabel.h \
    calibraframe.h \
    objeto.h

FORMS    += principal.ui

win32 { #CASO O SISTEMA OPERACIONAL FOR DO TIPO WINDOWS

    #DIRETORIO DOS HEADERS
    INCLUDEPATH += C:\opencv\build\include

    #DIRETORIO DAS RECURSOS A SEREM UTILIZADAS
    LIBS += C:\opencv\release\install\x64\mingw\bin\libopencv_calib3d248.dll
    LIBS += C:\opencv\release\install\x64\mingw\bin\libopencv_contrib248.dll
    LIBS += C:\opencv\release\install\x64\mingw\bin\libopencv_core248.dll
    LIBS += C:\opencv\release\install\x64\mingw\bin\libopencv_highgui248.dll
    LIBS += C:\opencv\release\install\x64\mingw\bin\libopencv_imgproc248.dll
}

unix { #CASO O SISTEMA OPERACIONAL FOR DO TIPO LINUX

    #DIRETORIO DOS HEADERS
    INCLUDEPATH += `pkg-config --cflags opencv`

    #DIRETORIO DAS RECURSOS A SEREM UTILIZADAS
    LIBS += `pkg-config --libs opencv`
}



