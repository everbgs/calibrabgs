#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

class Filmadora {

private:
	VideoCapture camera;
	int disp;
	string camDisp;
	void abrirCamera(void);
public:
	Filmadora(): disp(-2), camDisp(""){};
	void setDispositivo(int d);
	void setDispositivo(string s);
	void gravarVideoAVI(void);
	void gravarVideoMovimento(void);

};

void Filmadora::setDispositivo(int d){this->disp = d;}

void Filmadora::setDispositivo(string s){this->camDisp = s;}

void Filmadora::abrirCamera(void)
{
	if (this->camDisp != "")
		this->camera.open(this->camDisp);
	else if (disp > -2)
		this->camera.open(this->disp);
}

void Filmadora::gravarVideoAVI(void) 
{
	this->camera.release();
	this->abrirCamera();	

	if (!this->camera.isOpened())
		throw "Erro ao abrir camera para captura";

	double width = this->camera.get(CV_CAP_PROP_FRAME_WIDTH);
    double height = this->camera.get(CV_CAP_PROP_FRAME_HEIGHT); 
    Mat frame; 

	printf("Tamanho da Captura\n%.0f x %.0f\n", width, height); 
    namedWindow ("Camera", CV_WINDOW_AUTOSIZE);     

	VideoWriter video("video.avi", CV_FOURCC('D','I','V','3'), 20, cvSize((int)width, (int)height));
    if(!video.isOpened())
		throw "Não foi possivel criar o video capturado";
 
	puts("Press Esc to stop recording."); 	
    while(true)
    {
        this->camera >> frame; 		
		
        if(frame.empty())
			throw "Erro ao capturar frame";

        video << frame;

        imshow("Camera", frame);

		char c = waitKey(20); 
        if(c == 27) break;
    }
	destroyWindow("Camera");
}


void Filmadora::gravarVideoMovimento(void)
{
	this->camera.release();
	this->abrirCamera();	

	if (!this->camera.isOpened())
		throw "Erro ao abrir camera para captura";
	
    namedWindow ("Camera", CV_WINDOW_AUTOSIZE);         
    //namedWindow ("Camera2", CV_WINDOW_AUTOSIZE);         

	Mat frame, frameAnt; 	
	CvPoint minVal, maxVal;

	double width = this->camera.get(CV_CAP_PROP_FRAME_WIDTH);
    double height = this->camera.get(CV_CAP_PROP_FRAME_HEIGHT);
	VideoWriter video("mov.avi", CV_FOURCC('D','I','V','3'), 20, cvSize((int)width, (int)height));
    if(!video.isOpened())
		throw "Não foi possivel criar o video capturado";

	
	this->camera >> frameAnt; 			
	if (frameAnt.empty()) throw "Erro ao capturar frame";
	waitKey(20); 

	double val;

	while(true)
    {
        this->camera >> frame; 		
		
        if(frame.empty()) throw "Erro ao capturar frame";  

		blur(frame, frame, cv::Size(3,3));

		minVal.x = frame.size().width;
        minVal.y = frame.size().height;
        maxVal.x = 0;
        maxVal.y = 0;

		for (int i=0; i<frame.rows; i++)
		{
			for (int l=0; l<frame.cols; l++)		
			{
				val = (abs(frame.at<cv::Vec3b>(i,l)[0] - frameAnt.at<cv::Vec3b>(i,l)[0]) +
					  abs(frame.at<cv::Vec3b>(i,l)[1] - frameAnt.at<cv::Vec3b>(i,l)[1]) +
					  abs(frame.at<cv::Vec3b>(i,l)[2] - frameAnt.at<cv::Vec3b>(i,l)[2])) / 3;
				if (val > 35)
				{
					if(l<minVal.y)
		                minVal.y = l;
		            if(i<minVal.x)
		                minVal.x = i;
		            if(l>maxVal.y)
		                maxVal.y = l;
		            if(i>maxVal.x)
		                maxVal.x = i;
				}
			}
		}	
	
	    frameAnt = frame.clone();
		if (maxVal.x || maxVal.y)
		{
			video << frame;
			rectangle(frame, minVal, maxVal, cv::Scalar(0,0,255), 1, 8, 0);		
		}
	
        imshow("Camera", frame);
        //imshow("Camera2", frameAnt);
		char c = waitKey(20); 
		
        if(c == 27) break;
    }
	destroyWindow("Camera");
}

void cls(void);

int main()
{
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
			case 3:
				film.gravarVideoMovimento();				
				break;				
		}
	//	cls();
	}while (op != 0);	

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

