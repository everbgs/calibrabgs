#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h> 
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

class Filmadora {

private:
	/*Classe responsavel pela captura*/
	VideoCapture camera;

	/*Tipos de dispositivos, parametros:
		inteiro = Geralmente a WebCam ou cameras acopladas a maquina
		string = Cameras externas, Camera IP
	*/
	int intDisp;
	string strDisp;

	/*Atributos para mostrar o tempo*/
	char tempo_str[30];
  	time_t rawtime;
	struct tm * timeinfo;

	/*Abre Camera para captura*/
	void abrirCamera(void);

	/*Retorna dia/Mes/Ano Hora:Minuto:Segundo*/
	char* getDateTime(void);

	/*Retorna verdadeiro caso haja diferença entre as duas imagens*/
	bool isDetectouMovimento(Mat& m1, Mat& m2);
public:
	/*Informa o Dispositivo que vai ser utilizado*/
	void setDispositivo(int d);
	void setDispositivo(string s);

	/*Grava video da camera*/
	void gravarVideoCamera(string nomeArq);
	void gravarVideoCameraMovimento(string nomeArq);

	/*Função extra, grava somente o movimento de um arquivo de video*/
	void gravarVideoMovimentoArquivo(string nomeArq, string nameSave);
};

void Filmadora::abrirCamera(void)
{
	if (this->camera.isOpened())
		this->camera.release();	

	if (this->strDisp != "")
		this->camera.open(this->strDisp);
	else if (intDisp > -2)
		this->camera.open(this->intDisp);
}

char* Filmadora::getDateTime(void)
{
	time(&rawtime);  	
	timeinfo = localtime(&rawtime);
	strftime(tempo_str, 30, "%d/%m/%G %T", timeinfo);
	return tempo_str;
}

bool Filmadora::isDetectouMovimento(Mat& m1, Mat& m2)
{
	int nr = m1.rows;
	int nc = m1.cols;
	double val;

	for (int i=0; i<nr; i++)
	{
		for (int l=0; l<nc; l++)		
		{
			val = (abs(m1.at<cv::Vec3b>(i,l)[0] - m2.at<cv::Vec3b>(i,l)[0]) +
  				   abs(m1.at<cv::Vec3b>(i,l)[1] - m2.at<cv::Vec3b>(i,l)[1]) +
				   abs(m1.at<cv::Vec3b>(i,l)[2] - m2.at<cv::Vec3b>(i,l)[2])) / 3;
			if (val > 35)
				return true;
		}
	}
	return false;
}

void Filmadora::setDispositivo(int d)
{
	this->strDisp = "";
	this->intDisp = d;
}

void Filmadora::setDispositivo(string s)
{
	this->strDisp = s;
	this->intDisp = -2;
}

void Filmadora::gravarVideoCamera(string nomeArq)
{
	this->abrirCamera();

	if (!this->camera.isOpened())
		throw "Erro ao abrir camera para captura";

	double width = this->camera.get(CV_CAP_PROP_FRAME_WIDTH);
    double height = this->camera.get(CV_CAP_PROP_FRAME_HEIGHT); 
    Mat frame; 
	
	printf("Tamanho da Captura\n%.0f x %.0f\n", width, height); 
    namedWindow ("Camera", CV_WINDOW_AUTOSIZE);     

	nomeArq += ".avi";
	VideoWriter video(nomeArq, CV_FOURCC('D','I','V','3'), 20, cvSize((int)width, (int)height));
    if(!video.isOpened())
		throw "Não foi possivel criar o arquivo de video capturado";
 
	char tecla;
	puts("Pressione esc para sair."); 	
    while(this->camera.read(frame))
    {		
		video << frame;
		putText(frame, this->getDateTime(), Point(1, 40), FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar(255,255,255));
        imshow("Camera", frame);

		tecla = waitKey(20); 
        if (tecla == 27) break;
    }
	destroyWindow("Camera");
	this->camera.release();
}


void Filmadora::gravarVideoCameraMovimento(string nomeArq)
{
	this->abrirCamera();	

	if (!this->camera.isOpened())
		throw "Erro ao abrir camera para captura";

	double width = this->camera.get(CV_CAP_PROP_FRAME_WIDTH);
    double height = this->camera.get(CV_CAP_PROP_FRAME_HEIGHT);

	nomeArq += ".avi";
	VideoWriter video(nomeArq, CV_FOURCC('D','I','V','3'), 20, cvSize((int)width, (int)height));

    if(!video.isOpened())
		throw "Não foi possivel criar o video capturado";

	Mat frame, frameAnt; 	
	bool achou;	
	char teclado;

	this->camera >> frameAnt; 			
	if (!this->camera.read(frameAnt))
		throw "Erro ao capturar frame inicial";

	blur(frameAnt, frameAnt, cv::Size(3,3));
	waitKey(20); 
	
    namedWindow ("Camera", CV_WINDOW_AUTOSIZE);     
	puts("Pressione esc para sair.");     
	while(this->camera.read(frame))
    {
		blur(frame, frame, cv::Size(3,3));
		achou = this->isDetectouMovimento(frame, frameAnt);	
	
	    frameAnt = frame.clone();

		putText(frame, this->getDateTime(), Point(1, 40), FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar(255,255,255));
		if (achou)
		{
			video << frame;			
			putText(frame, "Gravando...", Point(1, 20), FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar(255,255,255));
		}
	
        imshow("Camera", frame);
		teclado = waitKey(20); 		
        if (teclado == 27) break;
    }
	destroyWindow("Camera");
	this->camera.release();
}

void Filmadora::gravarVideoMovimentoArquivo(string nomeArq, string nameSave)
{
	nomeArq += ".avi";
	VideoCapture arquivo(nomeArq);

	if (!arquivo.isOpened())
		throw "Erro ao abrir arquivo para captura";
	
	double width = arquivo.get(CV_CAP_PROP_FRAME_WIDTH);
    double height = arquivo.get(CV_CAP_PROP_FRAME_HEIGHT);
	
	nameSave += ".avi";
	VideoWriter video(nameSave, CV_FOURCC('D','I','V','3'), 20, cvSize((int)width, (int)height));
	if (!video.isOpened())
		throw "Não foi possivel criar o video capturado";
	
	Mat frame, frameAnt;	
	bool achou;

	if (!arquivo.read(frameAnt))
		throw "Não foi capturar o frame inicial";		

	blur(frameAnt, frameAnt, cv::Size(3,3));
	waitKey(30);

	namedWindow("Arquivo");	
	while(arquivo.read(frame))
    {		
		blur(frame, frame, cv::Size(3,3));
		achou = this->isDetectouMovimento(frame, frameAnt);	
	
	    frameAnt = frame.clone();

		putText(frame, this->getDateTime(), Point(1, 40), FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar(255,255,255));
		if (achou)
		{
			video << frame;			
			putText(frame, "Gravando...", Point(1, 20), FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar(255,255,255));
		}
	
        imshow("Camera", frame);
		waitKey(30); 		        
    }
	destroyWindow("Camera");
	arquivo.release();
}

void cls(void);

int main()
{

	/*	
	void gravarVideoCamera(string nomeArq);
	void gravarVideoCameraMovimento(string nomeArq);	
	void gravarVideoMovimentoArquivo(string nomeArq, string nameSave);
	*/
	Filmadora film;
	film.setDispositivo(0);
	film.gravarVideoMovimentoArquivo("video5", "novo2");

	/* Testar e terminar menu

	int op;
	Filmadora film;
	film.setDispositivo(0);

	do{			
		printf("*** Menu Principal ***\n"
				"1 - Gravar Video\n"
				"2 - Exibir Gravacao\n"
				"3 - Gravar Video (Movimento)\n"
				"4 - Calibrar Ruido\n"
				"0 - Sair\n-> ");
		scanf("%d", &op);
		switch(op)
		{
			case 1:
				film.gravarVideoAVI();				
				break;				
			case 2:
				film.exibirVideo();				
				break;				
			case 3:
				film.gravarVideoMovimento();				
				break;				
		}
	//	cls();
	}while (op != 0);	*/

	return 0;
}

void cls(void)
{
#ifdef 	__linux__
	system("clear");
#else
	system("cls");
#endif
}

