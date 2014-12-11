// Recognizer.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ComVision.h"

#include "OpenCVCapture.h"
#include "Recognize.h"

OpenCVCapture capture;
cv::Mat frame;


// This is an example of an exported variable
COMVISION_API int nRecognizer=0;

// This is an example of an exported function.
COMVISION_API int fnRecognizer(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see Recognizer.h for the class definition
CComVision::CComVision()
{
	return;
}


int CComVision::StartCapture(void)
{
	capture.StartCapture();
	return 0;
}

void CComVision::StopCapture(void)
{
	if(capture.IsRunning())
	{
		capture.StopCapture();
	}
}

void CComVision::Capture(void)
{
	if(capture.IsRunning())
	{
		frame = capture.GetImage();
	}
}

int CComVision::GetWidth(void)
{
	return frame.cols;
}
int CComVision::GetHeight(void)
{
	return frame.rows;
}
int CComVision::GetChannels(void)
{
	return frame.channels();
}

unsigned char* CComVision::GetRow(int i)
{
	return frame.ptr<unsigned char>(i);
}

bool CComVision::IsRunning()
{
	return capture.IsRunning();
}