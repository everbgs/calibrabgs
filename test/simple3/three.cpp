#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>


using namespace std;

cv::Mat frame; 

void onMouse(int evento, int x, int y, int, void*)
{
	if (evento != cv::EVENT_LBUTTONDOWN)
		return;

	if (frame.channels() == 1)	
		printf("[%d, %d] = Gray: %d\n", y, x,  frame.at<uchar>(y, x));
	else
	{
		printf("[%d, %d] = R: %d G: %d B: %d\n", y, x, 
			frame.at<cv::Vec3b>(y, x)[2],
				frame.at<cv::Vec3b>(y, x)[1],
					frame.at<cv::Vec3b>(y, x)[0]);		
	}
}

int main()
{

	int RMax, RMin, GMax, GMin, BMax, BMin;
	cv::Mat dst;
	RMax = 256;
	GMax = 100;	
	BMax = 100;
	

	RMin = 175;
	GMin = 0; 
	BMin = 0;	


	cv::VideoCapture webCam(0);
	if (!webCam.isOpened())
	{
		fprintf(stderr, "Erro ao iniciar camera");
		return -1;
	}	


	cv::namedWindow("Tela1");
	cv::setMouseCallback("Tela1", onMouse);

	cv::namedWindow("Tela2");
	cv::createTrackbar("Min R", "Tela2", &RMin, 255, 0);
    cv::createTrackbar("Min G", "Tela2", &GMin, 255, 0);
    cv::createTrackbar("Min B", "Tela2", &BMin, 255, 0);	   
	cv::createTrackbar("Max R", "Tela2", &RMax, 255, 0);
    cv::createTrackbar("Max G", "Tela2", &GMax, 255, 0);
    cv::createTrackbar("Max B", "Tela2", &BMax, 255, 0);

	while (true)
	{
		webCam >> frame;
		if (frame.empty()) break;
	
		inRange(frame, cv::Scalar(BMin, GMin, RMin), cv::Scalar(BMax, GMax, RMax), dst);
	    cv::GaussianBlur(dst, dst, cv::Size(9,9), 1.5);
		vector<cv::Vec3f> circles;
	    cv::HoughCircles(dst, circles, CV_HOUGH_GRADIENT, 2, dst.rows / 4, 100, 50, 10, 400);


		std::vector<cv::Vec3f>::const_iterator itc= circles.begin();
		while (itc!=circles.end()) 
		{
			cv::circle(frame, cv::Point((*itc)[0], (*itc)[1]), (*itc)[2], cv::Scalar(255), 2);
			++itc;
		}    

		cv::imshow("Tela1", frame);
		cv::imshow("Tela2", dst);


		char ch = cv::waitKey(20); 
	    if (ch == 27) break;	
	}

	cv::destroyWindow("Tela1");
	cv::destroyWindow("Tela2");	

    return 0;
}
