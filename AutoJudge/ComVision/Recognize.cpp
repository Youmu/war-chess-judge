#include "stdafx.h"
#include "Recognize.h"


Recognize::Recognize(void)
{
	_threshold = 40;
	_rect = cvRect(98, 79, 420, 270);
	_preprocessor = new Preprocessor(_threshold, _rect);
}


Recognize::~Recognize(void)
{
	delete _preprocessor;
	delete _classifier;
}


int Recognize::InitClassifier(std::string VocabularyNameFile, std::string ModelDir)
{
	 _classifier = new ChessClassifier(VocabularyNameFile, ModelDir);
	 _initialized = _classifier->initilize();
	 return 0;
}


std::string Recognize::RecognizeImage(IplImage* image)
{
	std::vector<IplImage*>* result = _preprocessor->ExtractChess(image);
	string label1 = _classifier->classify(result->at(0));
	string label2 = _classifier->classify(result->at(1));
	return label1 + "," + label2;
}
