#pragma once
class OpenCVCapture
{
private:
	bool _capture;
public:
	OpenCVCapture(void);
	~OpenCVCapture(void);
	int StartCapture(void);
	int StopCapture(void);
};

