#pragma once

#include <vector>
#include <string>

#include <iostream>
#include <filesystem>
#include <algorithm>
#include <opencv2\nonfree\features2d.hpp>
#include <opencv2\legacy\legacy.hpp>

#include "cv.h"
#include "highgui.h"

using namespace std;
using namespace cv;

class Preprocessor
{
public:
	int BinarizationThreshold;
	CvRect InterestRect;

	Preprocessor(void);
	Preprocessor(int binarizationThreshold, CvRect interestRect);
	~Preprocessor(void);

	std::vector<IplImage*>* ExtractChess(IplImage* image);
	void GenerateAllSamplesInFolder(string directory, string destination);
	void GenerateTrainingData(string srcDir, string detDir);
	void BuildVocabulary(string trainingDir, string serialDir, string vocabularyName);
	void Training(string trainingDir, string serialDir, string vocabularyName, string modelName);

private:
	void ExtractTrainingSet(string trainingDir, Ptr<FeatureDetector> &detector, BOWImgDescriptorExtractor &bowide, map<String, Mat> &trainingData);
	void TrainingSVM(map<string,Mat>& trainingData, int response_cols, int response_type, string serialDir, string modelName);

};

