#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h> 

using namespace cv;
 
int main (int argc, char *argv[])
{

	VideoCapture capture(0); 
    
    if(!capture.isOpened())
    {
		fprintf(stderr, "Não foi possivel abrir a camera para captura");
        return -1;
    } 

    double width = capture.get(CV_CAP_PROP_FRAME_WIDTH);
    double height = capture.get(CV_CAP_PROP_FRAME_HEIGHT); 

	printf("Tamanho da Captura\n%.0f x %.0f\n", width, height); 

    Mat frame; 

    namedWindow ("Tela", CV_WINDOW_AUTOSIZE); 
    
	VideoWriter video("video.avi", CV_FOURCC('P','I','M','1'), 20, cvSize((int)width,(int)height) );
    if(!video.isOpened())
    {
		fprintf(stderr, "Não foi possivel criar o video");
        return -1;
    }
 
	puts("Press Esc to stop recording."); 
    while(true)
    {
        capture >> frame; 

        if(frame.empty())
        {
            fprintf(stderr, "Erro ao capturar frame");
            return -1;
        } 

        video << frame;

        imshow("Tela", frame);

		char c = waitKey(20); 
        if(c == 27) break;
    }

    return 0;
}
