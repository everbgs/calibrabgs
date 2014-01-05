#include <opencv/cv.h>
#include <opencv/highgui.h>

int main( int argc, char** argv ) 
{
	char* nameFrm = "Teste 2";

	//Testando captura webcam
	cvNamedWindow( nameFrm, CV_WINDOW_AUTOSIZE );	
	CvCapture* capture = cvCaptureFromCAM(CV_CAP_ANY);		
	IplImage* frame;

	while (true) {
		frame = cvQueryFrame( capture );
		if( !frame ) break;
		cvShowImage( nameFrm, frame );
		
		char c = cvWaitKey(30);
		if( c == 27 ) break; //ESC para sair
	}

	cvReleaseCapture(&capture);
	cvDestroyWindow(nameFrm);

	return 0;
}
