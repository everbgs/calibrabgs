#include <opencv/cv.h>
#include <opencv/highgui.h>

int main(int argc, char** argv)
{
 
	//Carregar a imagem
	IplImage *img = cvLoadImage(argv[1], -1);
	
	//Cria uma janela para exibir a imagem
   	cvNamedWindow(argv[1], CV_WINDOW_AUTOSIZE);
	//Exibir a imagem
   	cvShowImage(argv[1], img);

	// Espera até fechar a janela
   	cvWaitKey(0);
	
	// Libera o objeto imagem
   	cvReleaseImage(&img);	
	
	// Destruir a janela
   	cvDestroyWindow(argv[1]);
	

	return 0;
}
