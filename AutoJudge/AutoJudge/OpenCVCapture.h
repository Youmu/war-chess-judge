#pragma once
class OpenCVCapture
{
private:
	bool _capture;
	bool _getImage;
	HANDLE _hevent;
	cv::Mat _image;
public:
	OpenCVCapture(void);
	~OpenCVCapture(void);
	int StartCapture(void);
	int StopCapture(void);
	cv::Mat GetImage(void);
};

