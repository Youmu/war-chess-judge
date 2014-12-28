#include "OpenCVCapture.h"
#include "iostream"
using namespace std;

void* capture_thr(void *arg);

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
	//Create capture Thread
	int err;
	pthread_cond_init(&m_cond, NULL);
	pthread_mutex_init(&m_mutex, NULL);
	err = pthread_create(&m_captid, NULL, capture_thr, this);
	return 0;
}

void* capture_thr(void* arg)
{	
	OpenCVCapture *caller = (OpenCVCapture*)arg;
	cv::VideoCapture vc(0);
	if(vc.isOpened() && caller->_capture)
	{
		while(caller->_capture)
		{
			cv::Mat frame;
			vc>>frame;
			pthread_mutex_lock(&caller->m_mutex);
			if(caller->_getImage)
			{
				caller->_getImage = false;
				caller->_image = frame.clone();
				pthread_cond_signal(&caller->m_cond);
				cout<<"signal m_cond"<<endl;
			}
			pthread_mutex_unlock(&caller->m_mutex);
		}
	}
	return 0;
}

int OpenCVCapture::StopCapture(void)
{
	void* tret;
	_capture = false;
	pthread_join(m_captid, &tret);
	pthread_cond_destroy(&m_cond);
	return 0;
}


cv::Mat OpenCVCapture::GetImage(void)
{
	if(_capture)
	{
		cout<<"lock mutex"<<endl;
		pthread_mutex_lock(&m_mutex);
		_getImage = true;
		cout<<"wait nutex"<<endl;
		pthread_cond_wait(&m_cond, &m_mutex);
		pthread_mutex_unlock(&m_mutex);
		return _image;
	}
	return cv::Mat();
}


bool OpenCVCapture::IsRunning(void)
{
	return _capture;
}
