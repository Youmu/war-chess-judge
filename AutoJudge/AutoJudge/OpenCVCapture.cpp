#include "stdafx.h"
#include "OpenCVCapture.h"


OpenCVCapture::OpenCVCapture(void)
{
	_capture = false;
}


OpenCVCapture::~OpenCVCapture(void)
{
}


int OpenCVCapture::StartCapture(void)
{
	// Create a thread to capture image.
	_capture = true;
	_getImage = false;
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
					if(caller->_getImage)
					{
						caller->_getImage = false;
						caller->_image = frame.clone();
						SetEvent(caller->_hevent);
					}
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


cv::Mat OpenCVCapture::GetImage(void)
{
	if(_capture)
	{
		_hevent = CreateEvent(NULL,TRUE,FALSE,TEXT("GetImageEvent"));
		_getImage = true;
		WaitForSingleObject(_hevent,INFINITE);
		CloseHandle(_hevent);
		return _image;
	}
	return cv::Mat();
}


bool OpenCVCapture::IsRunning(void)
{
	return _capture;
}
