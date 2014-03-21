#include <opencv/cv.h>
#include <opencv/highgui.h>

int main(int argc, char** argv)
{

	//Carregar a imagem
	IplImage *img = argc > 1 ? cvLoadImage(argv[1], -1) : cvLoadImage("lena.jpg", -1);	
	
	//Cria uma janela para exibir a imagem
   	cvNamedWindow("tela1", CV_WINDOW_AUTOSIZE);

	//Exibir a imagem
   	cvShowImage("tela1", img);

	// Espera até fechar a janela
   	cvWaitKey(0);
	
	// Libera o objeto imagem
   	cvReleaseImage(&img);	
	
	// Destruir a janela
   	cvDestroyWindow("tela1");
	

	return 0;
}
