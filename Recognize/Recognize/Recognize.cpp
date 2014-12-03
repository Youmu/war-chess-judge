// Recognize.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <string>
#include "Preprocessor.h"
#include "ChessClassifier.h"

using namespace std;
using namespace cv;


// Global variables
int g_threshold = 40;
CvRect g_rect = cvRect(98, 79, 420, 270);

void ClassifyImage(Preprocessor &preprocessor, ChessClassifier &classifier, string imageFile, bool showWindow = true, bool showBinarization = false)
{
	IplImage* image = cvLoadImage(imageFile.c_str());

	if(showWindow)
	{
		cvNamedWindow("Orignal", CV_WINDOW_AUTOSIZE);
		cvShowImage("Orignal", image);	
	}

	std::vector<IplImage*>* result = preprocessor.ExtractChess(image);

	if(showBinarization)
	{
		cvNamedWindow("Image1", CV_WINDOW_AUTOSIZE);
		cvNamedWindow("Image2", CV_WINDOW_AUTOSIZE);
		cvShowImage("Image1", result->at(0));	
		cvShowImage("Image2", result->at(1));	
	}

	string label1 = classifier.classify(result->at(0));
	string label2 = classifier.classify(result->at(1));

	std::cout << "Classify Results: " << label1 << " vs " << label2 << endl;

	if(showWindow || showBinarization) cvWaitKey(0);

	cvReleaseImage(&result->at(0));
	cvReleaseImage(&result->at(1));
	result->clear();
	cvReleaseImage(&image);

	if(showWindow || showBinarization) cvDestroyAllWindows();
}

void ClassifyDirectory(Preprocessor &preprocessor, ChessClassifier &classifier, string dir, bool showWindow = true, bool showBinarization = false)
{
	std::tr2::sys::path imagePath = dir;
	std::tr2::sys::recursive_directory_iterator end;
	std::tr2::sys::recursive_directory_iterator it(imagePath);
	for(; it != end; ++it)
	{
		auto& apath = it->path();
		string extension = apath.extension();
		for(auto& c : extension) c = toupper(c);
		if(is_regular_file(apath) && extension == ".JPG")
		{
			std::cout << apath.filename() << endl;
			string fullname = dir + "\\" + apath.filename();
			ClassifyImage(preprocessor, classifier, fullname, showWindow, showBinarization);
		}
	}
}

void PrintUsage()
{
	std::cout << "Usage:" << endl;
	std::cout << "Recognize.exe train SampleDir TrainDir SerializationDir VocabularyName ModelName" << endl;
	std::cout << "Recognize.exe classify SampleDir VocabularyNameFile ModelDir" << endl;
	std::cout << "Recognize.exe classify ImageFile VocabularyNameFile ModelDir" << endl;
}

int main(int argc, char* argv[])
{

	if(argc < 5)
	{
		PrintUsage();
		return 0;
	}

	if(strcmp(argv[1], "train") == 0)
	{
		if(argc != 7) PrintUsage();
		string sampleDir = argv[2];
		string trainDir = argv[3];
		string serializationDir = argv[4];
		string vocabularyName = argv[5];
		string modelName = argv[6];

		Preprocessor preprocessor(g_threshold, g_rect);

		preprocessor.GenerateTrainingData(sampleDir, trainDir);
		preprocessor.BuildVocabulary(trainDir, serializationDir, vocabularyName);
		preprocessor.Training(trainDir, serializationDir, vocabularyName, modelName);

		return 0;
	}

	if(strcmp(argv[1], "classify") == 0)
	{
		if(argc != 5) PrintUsage();
		string input = argv[2];
		string VocabularyNameFile = argv[3];
		string ModelDir = argv[4];

		std::tr2::sys::path imagePath = input;
		if(!std::tr2::sys::exists(imagePath))
		{
			std::cout << input << "doesn't exist." << endl;
			return -1;
		}

		Preprocessor preprocessor(g_threshold, g_rect);
		ChessClassifier classifier(VocabularyNameFile, ModelDir);
		bool init = classifier.initilize();
		if(!init)
		{
			std::cout << "Classifier initilize failed." << endl;
			return -1;
		}

		if(std::tr2::sys::is_directory(imagePath))
		{
			ClassifyDirectory(preprocessor, classifier, input, true, false);
		}
		if(std::tr2::sys::is_regular_file(imagePath))
		{
			ClassifyImage(preprocessor, classifier, input, true, true);
		}

		return 0;
	}

	PrintUsage();

	return 0;
}

