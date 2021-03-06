#ifndef PRINCIPAL_H
#define PRINCIPAL_H

#include <QWidget>
#include <QSlider>
#include <QMessageBox>
#include <QPlainTextEdit>
#include "camera.h"
#include "calibraframe.h"
#include "objeto.h"
#include <QFileDialog>
#include <QDebug>
#include "rastrearobjeto.h"
#include "camerathread.h"
#include "typesapp.h"
#include <opencv2/core/core.hpp>

namespace Ui {
    class Principal;
}

class Principal : public QWidget
{
    Q_OBJECT
    
public:
    explicit Principal(QWidget *parent = 0);
    ~Principal();

public slots:
    void processarFramesLocalizacao(QImage frame);
    void setCoordenadasLabel(QString nome, int x, int y);
    void setCoordenadasLabel(QString nome, int x, int y, double ang);

    void processarFramesCalibracao(QImage frame);    

    void doOnMouseDownImage(int x, int y);

    void setLabelFps(double fps);

    void setStatusThread(ThreadType m);

private slots:
    /* Eventos Visuais */
    void on_sliderMaxR_valueChanged(int value);
    void on_sliderMaxG_valueChanged(int value);
    void on_sliderMaxB_valueChanged(int value);
    void on_sliderMinR_valueChanged(int value);
    void on_sliderMinG_valueChanged(int value);
    void on_sliderMinB_valueChanged(int value);
    void on_cbCor_currentIndexChanged(int index);
    void on_btnCarregar_clicked();
    void on_btnIniciar_clicked();
    void on_btnMudarVisao_clicked();
    void on_btnExportar_clicked();
    void on_btnImportar_clicked();
    void on_rbRGBMax_clicked();
    void on_rbRGBMin_clicked();
    void on_cbkCirculo_clicked();
    void on_btnLocalizar_clicked();
    void on_btnImportLoc_clicked();

private:
    Ui::Principal *ui;    
    CalibraFrame* calibra;
    bool calibraClick;
    Objeto myObject;
    RastrearObjeto* rastrearObj;
    CameraThread* camThread;
    Camera* camera;

    void appendEditValueSlider(QPlainTextEdit* ed, QString value);
    void appendEditValueSlider(QSlider** sliders, QPlainTextEdit** edDestino, int n);
    void appendEditValueEdit(QPlainTextEdit** edOrigem, QPlainTextEdit** edDestino, int n);
    void changeThreadCalibra(int value, int channel, int range);
    _corcalibra getFormatCorCalibra(QPlainTextEdit** edts);
    void appendEditValueImport(QPlainTextEdit** edts, _corcalibra rgbCor);
    void appendPerspectivaSlider(int op);
};



#endif // PRINCIPAL_H
