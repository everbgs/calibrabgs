#include "principal.h"
#include "ui_principal.h"


Principal::Principal(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Principal){

    this->calibraClick = false;

    ui->setupUi(this);    
    connect(ui->lbImageCamera, SIGNAL(onMouseDown(int,int)), this, SLOT(doOnMouseDownImage(int,int)));

    this->camera = new Camera();

    this->calibra = new CalibraFrame(this);
    this->calibra->setCamera(this->camera);
    connect(this->calibra, SIGNAL(statusMethodThread(ThreadType)), this, SLOT(setStatusThread(ThreadType)));
    connect(this->calibra, SIGNAL(frameToQImage(QImage)), this, SLOT(processarFramesCalibracao(QImage)));

    this->rastrearObj = new RastrearObjeto(&this->myObject, this);
    connect(this->rastrearObj, SIGNAL(getObjCoordenadas(QString, int, int)), this, SLOT(setCoordenadasLabel(QString, int, int)));
    connect(this->rastrearObj, SIGNAL(getObjCoordenadas(QString, int, int, double)), this, SLOT(setCoordenadasLabel(QString, int, int, double)));

    this->camThread = new CameraThread(this);
    this->camThread->setCamera(this->camera);
    connect(this->camThread, SIGNAL(frameToQImage(QImage)), this, SLOT(processarFramesLocalizacao(QImage)));
    connect(this->camThread, SIGNAL(statusMethodThread(ThreadType)), this, SLOT(setStatusThread(ThreadType)));
    connect(this->camThread, SIGNAL(setFrameCapture(cv::Mat)), this->rastrearObj, SLOT(receberFrame(cv::Mat)));
}

Principal::~Principal()
{        
    while (this->calibra->isRunning())
        this->calibra->stop();
    while (this->camThread->isRunning())
        this->camThread->stop();

    delete this->calibra;
    delete this->camThread;
    delete this->rastrearObj;
    delete this->camera;

    delete ui;
}

void Principal::processarFramesLocalizacao(QImage frame)
{
    if (!frame.isNull())
    {
       //ui->lbCameraJogo->setAlignment(Qt::AlignCenter);
       ui->lbCameraJogo->setPixmap(QPixmap::fromImage(frame));
    }
}

void Principal::changeThreadCalibra(int value, int channel, int range)
{
    if (!this->calibra) return;

    //min = 0, max = 1
    switch (channel)
    {
        case 1: range ? this->calibra->RMax = value : this->calibra->RMin = value; break;
        case 2: range ? this->calibra->GMax = value : this->calibra->GMin = value; break;
        case 3: range ? this->calibra->BMax = value : this->calibra->BMin = value; break;
    }
}


void Principal::on_sliderMaxR_valueChanged(int value)
{
    this->changeThreadCalibra(value, 1, 1);
    this->appendPerspectivaSlider(1);
    this->appendEditValueSlider(ui->edMaxR, QString::number(value));
}

void Principal::on_sliderMaxG_valueChanged(int value)
{
    this->changeThreadCalibra(value, 2, 1);
    this->appendPerspectivaSlider(1);
    this->appendEditValueSlider(ui->edMaxG, QString::number(value));
}

void Principal::on_sliderMaxB_valueChanged(int value)
{
    this->changeThreadCalibra(value, 3, 1);
    this->appendPerspectivaSlider(1);
    this->appendEditValueSlider(ui->edMaxB, QString::number(value));
}

void Principal::on_sliderMinR_valueChanged(int value)
{
    this->changeThreadCalibra(value, 1, 0);
    this->appendPerspectivaSlider(0);
    this->appendEditValueSlider(ui->edMinR, QString::number(value));
}

void Principal::on_sliderMinG_valueChanged(int value)
{
    this->changeThreadCalibra(value, 2, 0);
    this->appendPerspectivaSlider(0);
    this->appendEditValueSlider(ui->edMinG, QString::number(value));
}

void Principal::on_sliderMinB_valueChanged(int value)
{
    this->changeThreadCalibra(value, 3, 0);
    this->appendPerspectivaSlider(0);
    this->appendEditValueSlider(ui->edMinB, QString::number(value));
}


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

void Principal::on_btnIniciar_clicked()
{    

    if (ui->btnIniciar->text() == "Parado")
    {
        ui->btnIniciar->setText("Iniciar");
        this->calibra->stop();
    }
    else
    {
        if (!this->camera->isCameraOpen())
        {
            ui->btnIniciar->setText("Parado");
            this->calibra->setExibeCirculo(ui->cbkCirculo->isChecked());
            this->calibra->play();
        }
    }    
}

void Principal::on_btnMudarVisao_clicked()
{    
    if (this->calibra)
        this->calibra->setVisaoRGB(!this->calibra->isVisaoRGB());
}

void Principal::processarFramesCalibracao(QImage frame)
{
    if (!frame.isNull())
    {               
       ui->lbImageCamera->setAlignment(Qt::AlignCenter);
       ui->lbImageCamera->setPixmap(QPixmap::fromImage(frame).scaled(ui->lbImageCamera->size()));
    }
}

void Principal::doOnMouseDownImage(int x, int y)
{
    if ((!ui->gbPerspectiva->isChecked())||
        (!ui->lbImageCamera->pixmap())) return;

    this->calibraClick = true;

    QImage img = ui->lbImageCamera->pixmap()->toImage();
    if (!img.isNull())
    {
        QRgb rgb = img.pixel(x, y);   
        QPalette palete;

        if (ui->rbRGBMax->isChecked())
        {
            ui->sliderMaxR->setValue(qRed(rgb));
            ui->sliderMaxG->setValue(qGreen(rgb));
            ui->sliderMaxB->setValue(qBlue(rgb));

            palete.setColor(ui->lbRGBPers->backgroundRole(), rgb);
            palete.setColor(ui->lbRGBPers->foregroundRole(), rgb);
            ui->lbRGBPers->setPalette(palete);
        }
        else if (ui->rbRGBMin->isChecked())
        {
            ui->sliderMinR->setValue(qRed(rgb));
            ui->sliderMinG->setValue(qGreen(rgb));
            ui->sliderMinB->setValue(qBlue(rgb));

            palete.setColor(ui->lbRGBPers->backgroundRole(), rgb);
            palete.setColor(ui->lbRGBPers->foregroundRole(), rgb);
            ui->lbRGBPers->setPalette(palete);
        }
    }
    this->calibraClick = false;
}

void Principal::setLabelFps(double fps)
{
    ui->lbFps->setText(QString::number(fps, 'g', '4'));
}

void Principal::setStatusThread(ThreadType m)
{
    if (m == CAL_FIRST)
    {
        if (!this->camera->isCameraOpen())
            this->camera->openCamera(__device);
    }
    else
    {
        if (this->camera->isCameraOpen())
            this->camera->stopCamera();
    }
}

_corcalibra Principal::getFormatCorCalibra(QPlainTextEdit** edts)
{
    _corcalibra colors;
    for (int i=0; i<6; i++)
        colors._cores[i] = edts[i]->toPlainText().toInt();
    return colors;
}

void Principal::appendEditValueImport(QPlainTextEdit **edts, _corcalibra rgbCor)
{
    for (int i=0; i<6; i++)
        this->appendEditValueSlider(edts[i], QString::number(rgbCor._cores[i]));
}


void Principal::on_btnExportar_clicked()
{
    QString dir =  QFileDialog::getSaveFileName(this, "Salvar arquivo", "", "Text files (*.txt)", 0);

    if (dir != "")
    {
        Objeto obj;

        if (ui->cbkAzul->isChecked())
        {
            QPlainTextEdit* edts[] = {ui->edMaxAzulR, ui->edMaxAzulG, ui->edMaxAzulB,
                                      ui->edMinAzulR, ui->edMinAzulG, ui->edMinAzulB};
            obj.setColor(cores::AZUL, this->getFormatCorCalibra(edts));
        }
        if (ui->cbkAmarelo->isChecked())
        {
            QPlainTextEdit* edts[] = {ui->edMaxAmaR, ui->edMaxAmaG, ui->edMaxAmaB,
                                      ui->edMinAmaR, ui->edMinAmaG, ui->edMinAmaB};
            obj.setColor(cores::AMARELO, this->getFormatCorCalibra(edts));
        }
        if (ui->cbkVerde->isChecked())
        {
            QPlainTextEdit* edts[] = {ui->edMaxVerR, ui->edMaxVerG, ui->edMaxVerB,
                                      ui->edMinVerR, ui->edMinVerG, ui->edMinVerB};
            obj.setColor(cores::VERDE, this->getFormatCorCalibra(edts));
        }
        if (ui->cbkRosa->isChecked())
        {
            QPlainTextEdit* edts[] = {ui->edMaxRosaR, ui->edMaxRosaG, ui->edMaxRosaB,
                                      ui->edMinRosaR, ui->edMinRosaG, ui->edMinRosaB};
            obj.setColor(cores::ROSA, this->getFormatCorCalibra(edts));
        }
        if (ui->cbkLaranja->isChecked())
        {
            QPlainTextEdit* edts[] = {ui->edMaxLaraR, ui->edMaxLaraG, ui->edMaxLaraB,
                                      ui->edMinLaraR, ui->edMinLaraG, ui->edMinLaraB};
            obj.setColor(cores::LARANJA, this->getFormatCorCalibra(edts));
        }

        try
        {
            obj.exportarArquivo(dir.toStdString());
        }catch(const char* msg){
            QMessageBox::information(this, "Exportar Arquivo", msg);
        }
    }
}

void Principal::on_btnImportar_clicked()
{
    QString dir =  QFileDialog::getOpenFileName(this, "Abrir arquivo", "", "Text files (*.txt)",0);
    if (dir != "")
    {
        Objeto obj;
        try
        {

            obj.importarArquivo(dir.toStdString());
            if (obj.isExistsColor(cores::AZUL))
            {
                QPlainTextEdit* edts[] = {ui->edMaxAzulR, ui->edMaxAzulG, ui->edMaxAzulB,
                                          ui->edMinAzulR, ui->edMinAzulG, ui->edMinAzulB};
                this->appendEditValueImport(edts, obj.getColor(cores::AZUL));
            }
            if (obj.isExistsColor(cores::AMARELO))
            {
                QPlainTextEdit* edts[] = {ui->edMaxAmaR, ui->edMaxAmaG, ui->edMaxAmaB,
                                          ui->edMinAmaR, ui->edMinAmaG, ui->edMinAmaB};
                this->appendEditValueImport(edts, obj.getColor(cores::AMARELO));
            }
            if (obj.isExistsColor(cores::VERDE))
            {
                QPlainTextEdit* edts[] = {ui->edMaxVerR, ui->edMaxVerG, ui->edMaxVerB,
                                          ui->edMinVerR, ui->edMinVerG, ui->edMinVerB};
                this->appendEditValueImport(edts, obj.getColor(cores::VERDE));
            }
            if (obj.isExistsColor(cores::ROSA))
            {
                QPlainTextEdit* edts[] = {ui->edMaxRosaR, ui->edMaxRosaG, ui->edMaxRosaB,
                                          ui->edMinRosaR, ui->edMinRosaG, ui->edMinRosaB};
                this->appendEditValueImport(edts, obj.getColor(cores::ROSA));
            }
            if (obj.isExistsColor(cores::LARANJA))
            {
                QPlainTextEdit* edts[] = {ui->edMaxLaraR, ui->edMaxLaraG, ui->edMaxLaraB,
                                          ui->edMinLaraR, ui->edMinLaraG, ui->edMinLaraB};
                this->appendEditValueImport(edts, obj.getColor(cores::LARANJA));
            }

        }catch(const char* msg){
            QMessageBox::information(this, "Importar Arquivo", msg);
        }
    }
}

void Principal::appendPerspectivaSlider(int op)
{
    if ((this->calibraClick)||(!ui->gbPerspectiva->isChecked()) ) return;

    QRgb rgb;
    QPalette palete;

    //op: 1 - MAX, 0 - MIN
    if ((op == 1)&&(ui->rbRGBMax->isChecked()))
    {
        rgb = qRgb(ui->sliderMaxR->value(), ui->sliderMaxG->value(), ui->sliderMaxB->value());
        palete.setColor(ui->lbRGBPers->backgroundRole(), rgb);
        palete.setColor(ui->lbRGBPers->foregroundRole(), rgb);
        ui->lbRGBPers->setPalette(palete);
    }
    else if ((!op)&&(ui->rbRGBMin->isChecked()))
    {
        rgb = qRgb(ui->sliderMinR->value(), ui->sliderMinG->value(), ui->sliderMinB->value());
        palete.setColor(ui->lbRGBPers->backgroundRole(), rgb);
        palete.setColor(ui->lbRGBPers->foregroundRole(), rgb);
        ui->lbRGBPers->setPalette(palete);
    }
}

void Principal::on_rbRGBMax_clicked()
{
    this->appendPerspectivaSlider(1);
}

void Principal::on_rbRGBMin_clicked()
{
    this->appendPerspectivaSlider(0);
}

void Principal::on_cbkCirculo_clicked()
{
    if (this->calibra)
        this->calibra->setExibeCirculo(ui->cbkCirculo->isChecked());

}

void Principal::setCoordenadasLabel(QString nome, int x, int y)
{
    ui->edCoordenadas->appendPlainText(nome +
                                       QString("X =" ) + QString::number(x).rightJustified(4, ' ') +
                                       QString(" Y =" ) + QString::number(y).rightJustified(4, ' '));
}

void Principal::setCoordenadasLabel(QString nome, int x, int y, double ang)
{

    ui->edCoordenadas->appendPlainText(nome +
                                       QString("X =" ) + QString::number(x).rightJustified(4, ' ') +
                                       QString(" Y =" ) + QString::number(y).rightJustified(4, ' ')+
                                       QString(" Ang =" ) + QString::number(ang).rightJustified(4, ' ')
                                       );
}

void Principal::on_btnLocalizar_clicked()
{
    if (ui->btnLocalizar->text() == "Parar")
    {
        ui->btnLocalizar->setText("Iniciar");
        this->camThread->stop();
    }
    else
    {
        if (!this->camera->isCameraOpen())
        {
            ui->btnLocalizar->setText("Parar");
            this->camThread->play();
        }
    }

}

void Principal::on_btnImportLoc_clicked()
{

    QString dir =  QFileDialog::getOpenFileName(this, "Abrir arquivo", "", "Text files (*.txt)",0);
    if (dir != "")
    {
        this->myObject.importarArquivo(dir.toStdString());
        QString file = dir.section("/",-1,-1);
        ui->lbArqUsado->setText("Arquivo: "+file);
    }
}
