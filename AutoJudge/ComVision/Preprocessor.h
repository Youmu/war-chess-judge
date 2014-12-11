#pragma once
#include "stdafx.h"
#include <vector>
#include <string>

#include <iostream>
#include <filesystem>
#include <algorithm>
#include <opencv2\nonfree\features2d.hpp>
#include <opencv2\legacy\legacy.hpp>

#include "opencv\cv.h"
#include "opencv/highgui.h"

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
	void GenerateAllSamplesInFolder(const string &directory, const string &destination);
	void GenerateTrainingData(const string &srcDir, const string &detDir);
	void BuildVocabulary(const string &trainingDir, const string &serialDir, const string &vocabularyName);
	void Training(const string &trainingDir, const string &serialDir, const string &vocabularyName, const string &modelName);

private:
	void ExtractTrainingSet(const string &trainingDir, Ptr<FeatureDetector> &detector, BOWImgDescriptorExtractor &bowide, map<String, Mat> &trainingData);
	void TrainingSVM(map<string,Mat>& trainingData, int response_cols, int response_type, const string &serialDir, const string &modelName);

};

