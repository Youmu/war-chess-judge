// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the RECOGNIZER_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// RECOGNIZER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef ComVision_EXPORTS
#define COMVISION_API __declspec(dllexport)
#else
#define COMVISION_API __declspec(dllimport)
#endif

// This class is exported from the Recognizer.dll
class COMVISION_API CComVision {

public:
	CComVision(void);
	// TODO: add your methods here.
	int StartCapture(void);
	void StopCapture(void);
	void Capture(void);
	int GetWidth(void);
	int GetHeight(void);
	int GetChannels(void);
	unsigned char* GetRow(int i);

	const char* Recognize();

	bool IsRunning();
};

extern COMVISION_API int nComVision;

COMVISION_API int fnComVision(void);
