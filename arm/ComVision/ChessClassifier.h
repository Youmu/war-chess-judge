#pragma once
#include "stdafx.h"
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <filesystem>

#include <opencv2\nonfree\features2d.hpp>
#include <opencv2\legacy\legacy.hpp>

#include "opencv\cv.h"
#include "opencv\highgui.h"

using namespace std;
using namespace cv;

class ChessClassifier
{
public:
	string VocabularyFile;
	string ModelPath;
	map<string, unique_ptr<CvSVM>>* Classifiers;
	Mat Vocabulary;
	Ptr<FeatureDetector> Detector;
	Ptr<DescriptorExtractor> Extractor;
	Ptr<DescriptorMatcher> Matcher;
	Ptr<BOWImgDescriptorExtractor> Bowide;
	
	ChessClassifier(string vocabularyFile, string modelPath);
	~ChessClassifier(void);

	bool initilize();
	string classify(IplImage* image);

private:
	void loadModel(string modelPath);

};

