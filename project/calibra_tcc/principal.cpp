#include "principal.h"
#include "ui_principal.h"

Principal::Principal(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Principal)
{
    ui->setupUi(this);
    this->calibra = new CalibraFrame(this);
    connect(this->calibra, SIGNAL(frameToQImage(QImage)), this, SLOT(processarFramesCalibracao(QImage)));
    this->calibra->start();
}

Principal::~Principal()
{    
    delete ui;
}


void Principal::on_sliderMaxR_valueChanged(int value){this->appendEditValueSlider(ui->edMaxR, QString::number(value));}
void Principal::on_sliderMaxG_valueChanged(int value){this->appendEditValueSlider(ui->edMaxG, QString::number(value));}
void Principal::on_sliderMaxB_valueChanged(int value){this->appendEditValueSlider(ui->edMaxB, QString::number(value));}

void Principal::on_sliderMinR_valueChanged(int value){this->appendEditValueSlider(ui->edMinR, QString::number(value));}
void Principal::on_sliderMinG_valueChanged(int value){this->appendEditValueSlider(ui->edMinG, QString::number(value));}
void Principal::on_sliderMinB_valueChanged(int value){this->appendEditValueSlider(ui->edMinB, QString::number(value));}


void Principal::appendEditValueSlider(QPlainTextEdit* ed, QString value)
{
    ed->clear();
    ed->appendPlainText(value);
}

void Principal::appendEditValueSlider(QSlider** sliders, QPlainTextEdit** edDestino, int n)
{
    for (int i = 0; i < n; i++)
        sliders[i]->setValue(edDestino[i]->toPlainText().toInt());
}

void Principal::appendEditValueEdit(QPlainTextEdit** edOrigem, QPlainTextEdit** edDestino, int n)
{
    for (int i = 0; i < n; i++)
        this->appendEditValueSlider(edDestino[i], edOrigem[i]->toPlainText());
}


void Principal::on_cbCor_currentIndexChanged(int index)
{
    QSlider* sliders[] =  {ui->sliderMaxR, ui->sliderMaxG, ui->sliderMaxB,
                           ui->sliderMinR, ui->sliderMinG, ui->sliderMinB};
    switch (index)
    {
        /*Azul*/
        case 0:
        {
            QPlainTextEdit* edts[] = {ui->edMaxAzulR, ui->edMaxAzulG, ui->edMaxAzulB,
                                      ui->edMinAzulR, ui->edMinAzulG, ui->edMinAzulB};
            this->appendEditValueSlider(sliders, edts, 6);
            break;
        }
        /*Amarelo*/
        case 1:
        {
            QPlainTextEdit* edts[] = {ui->edMaxAmaR, ui->edMaxAmaG, ui->edMaxAmaB,
                                      ui->edMinAmaR, ui->edMinAmaG, ui->edMinAmaB};
            this->appendEditValueSlider(sliders, edts, 6);
            break;
        }
        /*verde*/
        case 2:
        {
            QPlainTextEdit* edts[] = {ui->edMaxVerR, ui->edMaxVerG, ui->edMaxVerB,
                                      ui->edMinVerR, ui->edMinVerG, ui->edMinVerB};
            this->appendEditValueSlider(sliders, edts, 6);
            break;
        }
        /*Rosa*/
        case 3:
        {
            QPlainTextEdit* edts[] = {ui->edMaxRosaR, ui->edMaxRosaG, ui->edMaxRosaB,
                                      ui->edMinRosaR, ui->edMinRosaG, ui->edMinRosaB};
            this->appendEditValueSlider(sliders, edts, 6);
            break;
        }
        /*Laranja*/
        case 4:
        {
            QPlainTextEdit* edts[] = {ui->edMaxLaraR, ui->edMaxLaraG, ui->edMaxLaraB,
                                      ui->edMinLaraR, ui->edMinLaraG, ui->edMinLaraB};
            this->appendEditValueSlider(sliders, edts, 6);
            break;
        }
    }
}

void Principal::on_btnCarregar_clicked()
{
    int index = ui->cbCor->currentIndex();

    QPlainTextEdit* edOr[] =  {ui->edMaxR, ui->edMaxG, ui->edMaxB,
                               ui->edMinR, ui->edMinG, ui->edMinB};
    switch (index)
    {
        /*Azul*/
        case 0:
        {
            QPlainTextEdit* edts[] = {ui->edMaxAzulR, ui->edMaxAzulG, ui->edMaxAzulB,
                                      ui->edMinAzulR, ui->edMinAzulG, ui->edMinAzulB};
            this->appendEditValueEdit(edOr, edts, 6);
            break;

        }
        /*Amarelo*/
        case 1:
        {
            QPlainTextEdit* edts[] = {ui->edMaxAmaR, ui->edMaxAmaG, ui->edMaxAmaB,
                                      ui->edMinAmaR, ui->edMinAmaG, ui->edMinAmaB};
            this->appendEditValueEdit(edOr, edts, 6);
            break;
        }
        /*verde*/
        case 2:
        {
            QPlainTextEdit* edts[] = {ui->edMaxVerR, ui->edMaxVerG, ui->edMaxVerB,
                                      ui->edMinVerR, ui->edMinVerG, ui->edMinVerB};
            this->appendEditValueEdit(edOr, edts, 6);
            break;
        }
        /*Rosa*/
        case 3:
        {
            QPlainTextEdit* edts[] = {ui->edMaxRosaR, ui->edMaxRosaG, ui->edMaxRosaB,
                                      ui->edMinRosaR, ui->edMinRosaG, ui->edMinRosaB};
            this->appendEditValueEdit(edOr, edts, 6);
            break;
        }
        /*Laranja*/
        case 4:
        {
            QPlainTextEdit* edts[] = {ui->edMaxLaraR, ui->edMaxLaraG, ui->edMaxLaraB,
                                      ui->edMinLaraR, ui->edMinLaraG, ui->edMinLaraB};
            this->appendEditValueEdit(edOr, edts, 6);
            break;
        }
    }

}

CalibraFrame::CalibraFrame(QObject *parent): QThread(parent){}

void CalibraFrame::run()
{
    this->webCam = new Camera();

    this->webCam->openCamera(0);
    if (!this->webCam->isCameraOpen())
        return;

    while (true)
    {
        cv::Mat frame = this->webCam->nextFrame();
        if (frame.empty())
            return;

        cv::cvtColor(frame, frame, CV_BGR2RGB);
        QImage qimgOriginal( frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);

        emit frameToQImage(qimgOriginal);

        this->msleep(20);
    }
}

void Principal::processarFramesCalibracao(QImage image)
{
    ui->lbImageCamera->setPixmap(QPixmap::fromImage(image));
}

