#include "iostream"
#include "opencv2/opencv.hpp"
using namespace std;
int main()
{
	cv::VideoCapture vc(0);
	if(vc.isOpened())
	{
		cout<<"Opened"<<endl;
		for(int i = 0 ; i < 50; i++)
		{
			cv::Mat frame;
			vc>>frame;
			if(i == 49)
			{
				IplImage img = frame;
				cvSaveImage("test.bmp", &img);
			}
		}
	}
	else
	{
		cout<<"error"<<endl;
	}
return 0;
}
