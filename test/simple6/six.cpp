#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <time.h> 
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#define MAX_RUIDO_PERMITIDO 93

/*Descomentar linha abaixo, para utilizar a camera externa*/
//#define CAMERA_EXTERNA
#ifndef CAMERA_EXTERNA
	#define __device 0
#else
	#define __device "http://admin:admin@192.168.1.200/GetData.cgi?CH=2?resolution=800x592&req_fps=30&.mjpg"
#endif

using namespace std;
using namespace cv;

void cls(void);

void system_pause(const char *msg);


class Filmadora {
private:
	/*Classe responsavel pela captura*/
	VideoCapture camera;

	/*Atributos para mostrar o tempo*/
	char tempo_str[30];
  	time_t rawtime;
	struct tm * timeinfo;

	/*Retorna dia/Mes/Ano Hora:Minuto:Segundo*/
	char* getDateTime(void);

	/*Retorna verdadeiro caso haja diferença entre as duas imagens*/
	bool isDetectouMovimento(Mat& m1, Mat& m2, const char* nameTela);	

public:	
	~Filmadora();

	/*fecha a estrutura caso o dispositivo esteja aberto*/	
	void fecharCamera(void);
	
	/*Informa o Dispositivo/Arquivo que vai ser utilizado*/
	void setDispositivo(int d);
	void setDispositivo(string s);

	/*Grava video da camera*/
	void gravarVideoCamera(string nomeArq);

	/*Grava video da camera ou de algum arquivo .avi*/
	void gravarVideoCameraMovimento(string nomeArq);
};

Filmadora::~Filmadora()
{
	this->fecharCamera();
}

char* Filmadora::getDateTime(void)
{
	time(&rawtime);  	
	timeinfo = localtime(&rawtime);
	strftime(tempo_str, 30, "%d/%m/%Y %H:%M:%S", timeinfo);
	return tempo_str;
}

bool Filmadora::isDetectouMovimento(Mat& m1, Mat& m2, const char* nameTela)
{	
	Mat g1, g2, res;
	cvtColor(m1, g1, CV_BGR2GRAY);
	cvtColor(m2, g2, CV_BGR2GRAY);

	absdiff(g1, g2, res);
	threshold(res, res, 10, 255, CV_THRESH_BINARY);

	erode(res, res, 0);
	dilate(res, res, 0, Point(-1,-1), 2);
	erode(res, res, 0);	

	imshow(nameTela, res);

	return (countNonZero(res) > MAX_RUIDO_PERMITIDO);
}

void Filmadora::fecharCamera(void)
{
	if (this->camera.isOpened())
		this->camera.release();	
}

void Filmadora::setDispositivo(int d)
{
	this->fecharCamera();
	this->camera.open(d);
}

void Filmadora::setDispositivo(string s)
{
	this->fecharCamera();
	this->camera.open(s);
}

void Filmadora::gravarVideoCamera(string nomeArq)
{
	/* Se a camera não estiver aberta */
	if (!this->camera.isOpened())
	{
		fputs("Erro ao abrir camera para captura", stderr);
		return;
	}

	/*Dimensões que o dispositivo está capturando*/
	Size size = Size(this->camera.get(CV_CAP_PROP_FRAME_WIDTH),  
                     this->camera.get(CV_CAP_PROP_FRAME_HEIGHT));   	

	/*Classe que manipula os frames capturados*/
    Mat frame; 	

	/*Nome da Tela que exibirá a captura*/
    namedWindow ("Camera", CV_WINDOW_AUTOSIZE);     

	/*Classe Responsavel pela gravação dos frames*/
	nomeArq += ".avi";
	VideoWriter video(nomeArq, CV_FOURCC('D','I','V','3'), 20, size);

	/*Se não foi possivel criar o arquivo*/
    if(!video.isOpened())
	{
		fprintf(stderr, "Não foi possivel criar o arquivo de video capturado");
		return;
	}
 
	char tecla;
	puts("Pressione esc sobre a janela para sair."); 	

	/*Enquanto é possivel capturar frames*/
    while(this->camera.read(frame))
    {		
		/*Grava frame no arquivo de video através */
		video << frame;

		/*Exibe a data e hora nos frames da tela que exibirá a captura*/
		putText(frame, this->getDateTime(), Point(1, 40), FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar(255,255,255));
        imshow("Camera", frame);

		/*esc para sair*/
		tecla = waitKey(20); 
        if (tecla == 27) break;
    }
	destroyWindow("Camera");
	/*Fecha o dipositivo, pois ele não está sendo utilizada*/
	this->camera.release();
	system_pause("Video Normal");
}


void Filmadora::gravarVideoCameraMovimento(string nomeArq)
{
	if (!this->camera.isOpened())
	{
		fprintf(stderr, "Erro ao abrir dispositivo para captura");
		return;
	}		

	Size size = Size(this->camera.get(CV_CAP_PROP_FRAME_WIDTH),  
                     this->camera.get(CV_CAP_PROP_FRAME_HEIGHT));   	

	
	nomeArq += ".avi";
	VideoWriter video(nomeArq, CV_FOURCC('D','I','V','3'), 20, size);

    if(!video.isOpened())
	{
		fprintf(stderr, "Não foi possivel criar o video capturado");
		return;
	}		


	Mat frame, frameAnt; 	
	bool achou;	
	char teclado;

	if (!this->camera.read(frameAnt))
		throw "Erro ao capturar frame inicial";

	blur(frameAnt, frameAnt, cv::Size(3,3));
	waitKey(20); 
	
    namedWindow ("Camera", CV_WINDOW_AUTOSIZE);     
    namedWindow ("Movimento", CV_WINDOW_AUTOSIZE);     

	puts("Pressione esc para sair.");     
	while(this->camera.read(frame))
    {
		blur(frame, frame, cv::Size(3,3));
		achou = this->isDetectouMovimento(frame, frameAnt, "Movimento");	
	
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
	destroyWindow("Movimento");

	this->camera.release();
	system_pause("Video Movimento");
}

int main()
{
	int op;
	Filmadora film;
	char strName[100], strFile[100];
	do{			
		cls();
		printf("*** Menu Principal ***\n"
				"1 - Gravar video com camera\n"
				"2 - Gravar somente movimento com camera\n"
				"3 - Gravar somente movimento com arquivo\n"
				"0 - Sair\n-> ");
		scanf("%d%*c", &op);	
		if ((op == 1)||(op == 2)||(op == 3))
		{
			printf("Informe o nome do arquivo que sera salvo(Max 100)\n-> ");
			fgets(strName, 100, stdin);
			strName[strlen(strName)-1] = '\0';
		}
		switch(op)
		{
			case 1:
				film.setDispositivo(__device);
				film.gravarVideoCamera(strName);				
				break;				
			case 2:
				film.setDispositivo(__device);				
				film.gravarVideoCameraMovimento(strName);				
				break;				
			case 3:
				printf("Informe o nome do arquivo.avi que sera capturado(Max 100)\n-> ");
				fgets(strFile, 100, stdin);
				strFile[strlen(strFile)-1] = '\0';

				film.setDispositivo(strFile);
				film.gravarVideoCameraMovimento(strName);								
				break;				
		}		
	} while (op != 0);	
	
	return 0;
}

void system_pause(const char *msg)
{
	char lixo[100];
	printf("\nOperacaoo concluida: %s\nPressione qualquer tecla para sair...\n", msg);
	fgets(lixo, 100, stdin);
}

void cls(void)
{
#ifdef 	__linux__
	system("clear");
#else
	system("cls");
#endif
}
