#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

int main(int argc, char *argv[])
{
	cv::Mat img = cv::Mat::zeros(500, 600, CV_8UC3);

	cv::rectangle(img, cv::Point(200,50), cv::Point(315, 150), cv::Scalar(0,0,200), 3, 4);

	cv::rectangle(img, cv::Point(200,200), cv::Point(300, 300), cv::Scalar(0,200,0), 5, 8);

	cv::rectangle(img, cv::Point(200,350), cv::Point(300, 450), cv::Scalar(200,0,0), -1, CV_AA);

	cv::namedWindow("RETANGULO", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
	cv::imshow("RETANGULO", img);

	cv::waitKey(0);
	return 0;
}
