#include "stdafx.h"
#include "OpenCVCapture.h"


OpenCVCapture::OpenCVCapture(void)
{

}


OpenCVCapture::~OpenCVCapture(void)
{
}


int OpenCVCapture::StartCapture(void)
{
	// Create a thread to capture image.
	_capture = true;
	CWinThread *thread = AfxBeginThread(
		[](LPVOID arg)
		{
			OpenCVCapture *caller = (OpenCVCapture*)arg;
			cv::VideoCapture vc(0);
			if(vc.isOpened() && caller->_capture)
			{
				while(caller->_capture)
				{
					cv::Mat frame;
					vc>>frame;
				}
			}
			return (UINT)0;
		},
		this
		);
	return 0;
}


int OpenCVCapture::StopCapture(void)
{
	_capture = false;
	return 0;
}
