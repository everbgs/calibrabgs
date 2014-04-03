#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdio.h>
#define BGR_MAX_SLIDER 255

using namespace cv;


int main()
{
    int R = 0, G = 0, B = 0;
    Mat src;
    Mat dst;

    src = imread("lena.jpg");

    if( !src.data ) { fprintf(stderr, "Error ao carregar imagem\n"); return -1; }

    namedWindow("Tela", 1);
    createTrackbar( "R", "Tela", &R, BGR_MAX_SLIDER, NULL );
    createTrackbar( "G", "Tela", &G, BGR_MAX_SLIDER, NULL );
    createTrackbar( "B", "Tela", &B, BGR_MAX_SLIDER, NULL );

    imshow( "Tela", src);

    waitKey(0);
    destroyWindow("Tela");

    return 0;
}
