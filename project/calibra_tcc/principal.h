#ifndef PRINCIPAL_H
#define PRINCIPAL_H

#include <QWidget>
#include <QSlider>
#include <QMessageBox>
#include <QPlainTextEdit>
#include "camera.h"
#include "calibraframe.h"

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
    void processarFramesCalibracao(QImage image);
    void doOnMouseDownImage(int x, int y);

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

private:
    Ui::Principal *ui;    
    CalibraFrame* calibra;

    void appendEditValueSlider(QPlainTextEdit* ed, QString value);
    void appendEditValueSlider(QSlider** sliders, QPlainTextEdit** edDestino, int n);
    void appendEditValueEdit(QPlainTextEdit** edOrigem, QPlainTextEdit** edDestino, int n);
};



#endif // PRINCIPAL_H
