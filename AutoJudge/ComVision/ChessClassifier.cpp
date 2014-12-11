#include "stdafx.h"
#include "ChessClassifier.h"


ChessClassifier::ChessClassifier(string vocabularyFile, string modelPath)
{
	VocabularyFile = vocabularyFile;
	ModelPath = modelPath;
}


ChessClassifier::~ChessClassifier(void)
{
	if(Classifiers != NULL) delete Classifiers;
}

bool ChessClassifier::initilize()
{
	std::cout << "Loading Model ..." << endl;
	loadModel(ModelPath);

	std::tr2::sys::path vocabularyPath = VocabularyFile;
	if(!exists(vocabularyPath)){
		std::cout << "The path " << VocabularyFile << " doesn't exist." << endl;
		return false;
	}

	std::cout << "Read vocabulary from " << VocabularyFile << endl;
	FileStorage fs(VocabularyFile, FileStorage::READ);
	fs["vocabulary"] >> Vocabulary;

	Ptr<FeatureDetector> _Detector(new SurfFeatureDetector());
	Ptr<DescriptorExtractor> _Extractor(new SurfDescriptorExtractor());
	Ptr<DescriptorMatcher> _Matcher(new BruteForceMatcher<L2<float>>());
	Detector = _Detector;
	Extractor = _Extractor;
	Matcher = _Matcher;

	Ptr<BOWImgDescriptorExtractor> _Bowide(new BOWImgDescriptorExtractor(Extractor, Matcher));
	Bowide = _Bowide;
	Bowide->setVocabulary(Vocabulary);

	return true;
}

void ChessClassifier::loadModel(string modelPath)
{
	Classifiers = new map<string, unique_ptr<CvSVM>>();
	std::tr2::sys::path modelFolder = modelPath;
	if(!exists(modelFolder)){
		std::cout << "The path " << modelPath << " doesn't exist." << endl;
		return;
	}

	std::tr2::sys::recursive_directory_iterator end;
	std::tr2::sys::recursive_directory_iterator it(modelFolder);
	int index = 0;
	for(; it != end; ++it)
	{
		auto& apath = it->path();
		string extension = apath.extension();
		for(auto& c : extension) c = toupper(c);
		if(is_regular_file(apath) && extension == ".YML")
		{
			string filename = apath.filename();
			string label = filename.substr(
				filename.find_last_of('_') + 1,
				filename.find_last_of('.') - filename.find_last_of('_') - 1
				);
			Classifiers->insert(make_pair(label, unique_ptr<CvSVM>(new CvSVM())));
			(*Classifiers)[label]->load(apath.string().c_str());
		}
	}
	return;
}

string ChessClassifier::classify(IplImage* image)
{
	std::cout << "Extracting features ..." << endl;
	Mat img(image);
	vector<KeyPoint> keypoints;
	Mat response_hist;
	Detector->detect(img, keypoints);
	Bowide->compute(img, keypoints, response_hist);

	std::cout << "Classifying ..." << endl;
	float minf = FLT_MAX;
	string minClass = "Nothing";
	for(map<string,unique_ptr<CvSVM>>::iterator it = Classifiers->begin(); it != Classifiers->end(); it++)
	{
		float res = abs((*it).second->predict(response_hist, true));
		std::cout << "Label: " << (*it).first << " - Result: " << res << endl;
		// not this class
		if(res > 1) continue;
		if(res < minf){
			minf = res;
			minClass = (*it).first;
		}
	}

	std::cout << "Class: " << minClass << " (" << minf << ")" << endl;

	return minClass;

}