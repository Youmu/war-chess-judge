#include "stdafx.h"
#include "OpenCVCapture.h"


OpenCVCapture::OpenCVCapture(void)
{
	_capture = false;
	_thread = nullptr;
}


OpenCVCapture::~OpenCVCapture(void)
{
}


int OpenCVCapture::StartCapture(void)
{
	// Create a thread to capture image.
	_capture = true;
	_getImage = false;
	_thread = AfxBeginThread(
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
	::WaitForSingleObject(_thread->m_hThread, INFINITE);
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
