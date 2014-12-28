#include "iostream"
#include "opencv2/opencv.hpp"
#include "ComVision/OpenCVCapture.h"
using namespace std;
int main()
{
	OpenCVCapture cvcapt;
	cout<<"Opened"<<endl;
	cvcapt.StartCapture();
	string cmd;
	int i = 0;
	do
	{	
		i++;
		cin>>cmd;
		IplImage img = cvcapt.GetImage();
		cout<<"Captured"<<endl;
		cvSaveImage((cmd+".bmp").c_str(), &img);
	}while(i<3);
	cout<<"Done!"<<endl;
return 0;
}
