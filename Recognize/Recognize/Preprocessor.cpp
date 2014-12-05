#include "stdafx.h"
#include "Preprocessor.h"


Preprocessor::Preprocessor(void)
{
	// Default initalization value
	BinarizationThreshold = 40;
	InterestRect = cvRect(98, 79, 420, 270);
}

Preprocessor::Preprocessor(int binarizationThreshold, CvRect interestRect)
{
	BinarizationThreshold = binarizationThreshold;
	InterestRect = interestRect;
}


Preprocessor::~Preprocessor(void)
{
}

std::vector<IplImage*>* Preprocessor::ExtractChess(IplImage* image)
{
	cvSetImageROI(image, InterestRect);
	IplImage* roi_img = cvCreateImage(cvSize(InterestRect.width, InterestRect.height), IPL_DEPTH_8U, 1);
	cvCvtColor(image, roi_img, CV_RGB2GRAY);

	IplImage* binary_img = cvCreateImage(cvSize(InterestRect.width, InterestRect.height), IPL_DEPTH_8U, 1);
	cvThreshold(roi_img, binary_img, BinarizationThreshold, 255, CV_THRESH_BINARY);

	CvRect leftRect = cvRect(0, 0, InterestRect.width / 2, InterestRect.height);
	IplImage* left_img = cvCreateImage(cvSize(leftRect.width, leftRect.height), IPL_DEPTH_8U, 1);
	CvRect rightRect = cvRect(InterestRect.width / 2, 0, InterestRect.width / 2 - 1, InterestRect.height);
	IplImage* right_img = cvCreateImage(cvSize(rightRect.width, rightRect.height), IPL_DEPTH_8U, 1);
	
	cvSetImageROI(binary_img, leftRect);
	cvCopy(binary_img, left_img);
	cvResetImageROI(binary_img);

	cvSetImageROI(binary_img, rightRect);
	cvCopy(binary_img, right_img);
	cvResetImageROI(binary_img);
	
	cvReleaseImage(&roi_img);
	cvReleaseImage(&binary_img);

	std::vector<IplImage*>* result = new std::vector<IplImage*>();
	result->push_back(left_img);
	result->push_back(right_img);

	return result;
}

void Preprocessor::GenerateAllSamplesInFolder(const string &directory, const string &destination)
{
	std::tr2::sys::path imagePath = directory;
	if(!exists(imagePath)){
		std::cout << "The path " << directory << " doesn't exist." << endl;
		return;
	}
	if(!is_directory(imagePath)){
		std::cout << "The path " << directory << " is not a directory." << endl;
		return;
	}

	std::tr2::sys::path detPath = destination;
	if(!exists(detPath))
	{
		std::tr2::sys::create_directory(detPath);
	}

	std::tr2::sys::recursive_directory_iterator end;
	std::tr2::sys::recursive_directory_iterator it(imagePath);
	int index = 0;
	for(; it != end; ++it)
	{
		auto& apath = it->path();
		string extension = apath.extension();
		for(auto& c : extension) c = toupper(c);
		if(is_regular_file(apath) && extension == ".JPG")
		{
			std::cout << apath.filename() << endl;

			string fullname = directory + "\\" + apath.filename();
			IplImage* image = cvLoadImage(fullname.c_str());
			// cvShowImage("Orignal", image);
			std::vector<IplImage*>* result = ExtractChess(image);
			for(auto& img : *result)
			{
				string filename = destination + "\\" + std::to_string(index++) + ".JPG";
				cvSaveImage(filename.c_str(), img);
				cvReleaseImage(&img);
			}
			result->clear();
			cvReleaseImage(&image);
		}
	}

}

void Preprocessor::GenerateTrainingData(const string &srcDir, const string &detDir)
{
	std::tr2::sys::path srcPath = srcDir;
	if(!exists(srcPath)){
		std::cout << "The path " << srcDir << " doesn't exist." << endl;
		return;
	}
	if(!is_directory(srcPath)){
		std::cout << "The path " << srcDir << " is not a directory." << endl;
		return;
	}

	std::tr2::sys::path detPath = detDir;
	if(!exists(detPath)){
		std::cout << "The path " << detDir << " doesn't exist." << endl;
		return;
	}
	if(!is_directory(detPath)){
		std::cout << "The path " << detDir << " is not a directory." << endl;
		return;
	}

	std::tr2::sys::recursive_directory_iterator end;
	std::tr2::sys::recursive_directory_iterator it(srcPath);
	int index = 0;
	for(; it != end; ++it)
	{
		auto& apath = it->path();
		if(is_directory(apath))
		{
			string detFullPath = detDir + "\\" + apath.filename();
			std::tr2::sys::path detFullFolder = detFullPath;
			if(!exists(detFullFolder))
			{
				std::tr2::sys::create_directory(detFullFolder);
			}
			GenerateAllSamplesInFolder(apath.string(), detFullPath);
		}
	}
}

void Preprocessor::BuildVocabulary(const string &trainingDir, const string &serialDir, const string &vocabularyname)
{
	std::tr2::sys::path trainingPath = trainingDir;
	if(!exists(trainingPath)){
		std::cout << "The path " << trainingDir << " doesn't exist." << endl;
		return;
	}
	if(!is_directory(trainingPath)){
		std::cout << "The path " << trainingDir << " is not a directory." << endl;
		return;
	}

	SurfFeatureDetector detector(400);
	vector<KeyPoint> keypoints;

	Ptr<DescriptorExtractor> extractor(new SurfDescriptorExtractor());
	Mat descriptors;
	Mat training_Descriptors(1, extractor->descriptorSize(), extractor->descriptorType());

	std::cout << "Build vocabulary ..." << endl;
	std::cout << "Extract descriptors ..." << endl;

	std::tr2::sys::recursive_directory_iterator end;
	std::tr2::sys::recursive_directory_iterator it(trainingPath);
	int index = 0;
	for(; it != end; ++it)
	{
		auto& apath = it->path();
		string extension = apath.extension();
		for(auto& c : extension) c = toupper(c);
		if(is_regular_file(apath) && extension == ".JPG")
		{
			std::cout << apath.string() << endl;

			string fullname = apath.string();
			IplImage* image = cvLoadImage(fullname.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
			Mat img(image);
			detector.detect(img, keypoints);

			/*
			Mat out;
			drawKeypoints(img, keypoints, out, Scalar(255));
			imshow("keypoints", out);
			waitKey(0);
			*/

			extractor->compute(img, keypoints, descriptors);
			training_Descriptors.push_back(descriptors);

			cvReleaseImage(&image);
		}
	}
	std::cout << "Total descriptors:" << training_Descriptors.rows << endl;

	/*
	FileStorage fs1(serialDir + "\\training_descriptors.yml", FileStorage::WRITE);
	fs1 << "training_descriptors" << training_Descriptors;
	fs1.release();
	*/

	BOWKMeansTrainer bowTrainer(1000);
	bowTrainer.add(training_Descriptors);
	std::cout << "Cluster BOW features" << endl;
	Mat vocabulary = bowTrainer.cluster();

	FileStorage fs2(serialDir + "\\" + vocabularyname, FileStorage::WRITE);
	fs2 << "vocabulary" << vocabulary;
	fs2.release();
}

void Preprocessor::ExtractTrainingSet(const string &trainingDir, Ptr<FeatureDetector> &detector, BOWImgDescriptorExtractor &bowide, map<String, Mat> &trainingData)
{
	vector<string> classes;
	std::tr2::sys::path trainingPath = trainingDir;
	std::tr2::sys::recursive_directory_iterator end;
	std::tr2::sys::recursive_directory_iterator it(trainingPath);
	for(; it != end; ++it)
	{
		auto& apath = it->path();
		if(is_directory(apath))
		{
			classes.push_back(apath.filename());
		}
	}

	std::cout << "Find " << classes.size() << " classes." << endl;

	for(string label : classes){
		vector<KeyPoint> keypoints; 
		Mat response_hist; 
		string sampleFolder = trainingDir + "\\" + label;

		std::cout << "Handling class " << label << endl;

		std::tr2::sys::path trainingPath = sampleFolder;
		std::tr2::sys::recursive_directory_iterator end;
		std::tr2::sys::recursive_directory_iterator it(sampleFolder);
		int index = 0;
		for(; it != end; ++it)
		{
			auto& apath = it->path();
			string extension = apath.extension();
			for(auto& c : extension) c = toupper(c);
			if(is_regular_file(apath) && extension == ".JPG")
			{
				std::cout << apath.string() << endl;
				IplImage* image = cvLoadImage(apath.string().c_str(), CV_LOAD_IMAGE_GRAYSCALE);
				Mat img(image);
				detector->detect(img, keypoints);
				bowide.compute(img, keypoints, response_hist);
				if(trainingData.count(label) == 0)
				{
					trainingData[label].create(0, response_hist.cols, response_hist.type());
				}
				trainingData[label].push_back(response_hist);
				cvReleaseImage(&image);
			}
		}
	}
}

// 1-to-others SVM
void Preprocessor::TrainingSVM(map<string,Mat>& trainingData, int response_cols, int response_type, const string &serialDir, const string &modelName)
{
	vector<string> classes;
	for(map<string,Mat>::iterator it = trainingData.begin(); it != trainingData.end(); it++)
	{
		classes.push_back((*it).first);
	}

	for(int i = 0;i < classes.size();i++)
	{
		string label = classes[i];
		std::cout << "training class " << label << " ..." << endl;
		Mat samples(0, response_cols, response_type);
		Mat labels(0, 1, CV_32FC1);

		// positive
		std::cout << "Adding " << trainingData[label].rows << " positive samples." << endl;
		samples.push_back(trainingData[label]);
		Mat class_label = Mat::zeros(trainingData[label].rows, 1, CV_32FC1);
		labels.push_back(class_label);

		//negative
		int negCount = 0;
		// select maxSamples samples from each negative set due to balance samples
		int maxSamples = 5;
		for(map<string,Mat>::iterator it = trainingData.begin(); it != trainingData.end(); it++)
		{
			string not_label = (*it).first;
			if(not_label.compare(label) == 0) continue;
			for(int i = 0; i < maxSamples; i++)
			{
				negCount++;
				samples.push_back(trainingData[not_label].row(i));
			}
		}
		class_label = Mat::ones(negCount, 1, CV_32FC1);
		labels.push_back(class_label);

		std::cout << "Adding " << negCount << " negative samples." << endl;

		std::cout << "Training SVM ..." << endl;
		Mat samples_32f;
		samples.convertTo(samples_32f, CV_32F);
		CvSVM classifier;
		classifier.train(samples_32f, labels);

		string modelFolder = serialDir + "\\" + modelName;
		std::tr2::sys::path modelPath = modelFolder;
		if(!exists(modelPath))
		{
			std::tr2::sys::create_directory(modelPath);
		}
		string classifierFile = serialDir + "\\" + modelName + "\\" + modelName + "_" + label + ".yml";
		classifier.save(classifierFile.c_str());
		std::cout << "SVM model saved." << endl;
	}
}

void Preprocessor::Training(const string &trainingDir, const string &serialDir, const string &vocabularyName, const string &modelName)
{
	std::tr2::sys::path trainingPath = trainingDir;
	if(!exists(trainingPath)){
		std::cout << "The path " << trainingDir << " doesn't exist." << endl;
		return;
	}
	if(!is_directory(trainingPath)){
		std::cout << "The path " << trainingDir << " is not a directory." << endl;
		return;
	}

	string vocabularyFile = serialDir + "\\" + vocabularyName;
	std::tr2::sys::path vocabularyPath = vocabularyFile;
	if(!exists(vocabularyPath)){
		std::cout << "The path " << vocabularyFile << " doesn't exist." << endl;
		return;
	}

	std::cout << "Start training ..." << endl;
	std::cout << "Read vocabulary from " << vocabularyFile << endl;
	Mat vocabulary;
	FileStorage fs(vocabularyFile, FileStorage::READ);
	fs["vocabulary"] >> vocabulary;

	Ptr<FeatureDetector> detector(new SurfFeatureDetector());
	Ptr<DescriptorExtractor> extractor(new SurfDescriptorExtractor());
	Ptr<DescriptorMatcher> matcher(new BruteForceMatcher<L2<float>>());
	BOWImgDescriptorExtractor bowide(extractor, matcher);
	bowide.setVocabulary(vocabulary);

	map<String, Mat> trainingData;
	trainingData.clear();

	std::cout << "Extract training data ..." << endl;
	ExtractTrainingSet(trainingDir, detector, bowide, trainingData);
	std::cout << "Got " << trainingData.size() << " classes." << endl;
	for(map<string, Mat>::iterator it = trainingData.begin(); it != trainingData.end(); it++)
	{
		std::cout << "Class " << (*it).first << " has " << (*it).second.rows << " samples." << endl;
	}

	std::cout << "Stat training SVMs ..." << endl;
	TrainingSVM(trainingData, bowide.descriptorSize(), bowide.descriptorType(), serialDir, modelName);

}


