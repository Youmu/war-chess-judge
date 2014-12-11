#pragma once
#include "Preprocessor.h"
#include "ChessClassifier.h"
class Recognize
{
private:
	int _threshold;
	CvRect _rect;
	Preprocessor* _preprocessor;
	ChessClassifier* _classifier;
	bool _initialized; 
public:
	Recognize(void);
	~Recognize(void);
	int InitClassifier(std::string VocabularyNameFile, std::string ModelDir);
	std::string RecognizeImage(IplImage* image);
};

