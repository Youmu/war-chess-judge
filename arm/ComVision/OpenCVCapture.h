#include "opencv2/opencv.hpp"
#include <pthread.h>
class OpenCVCapture
{
private:
	bool _capture;
	bool _getImage;
//	HANDLE _hevent;
	//The TID of the capture thread.
	pthread_t m_captid;
	pthread_cond_t m_cond;
	pthread_mutex_t m_mutex;
	cv::Mat _image;
	friend void* capture_thr(void* arg);	
public:
	OpenCVCapture(void);
	~OpenCVCapture(void);
	int StartCapture(void);
	int StopCapture(void);
	cv::Mat GetImage(void);
	bool IsRunning(void);
};

