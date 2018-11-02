#pragma once
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/face.hpp"
#include <vector>


class FaceTracker
{
public:
	FaceTracker();
	~FaceTracker();

	void GetLandmark(float * buf, int size);
private:
	
	cv::VideoCapture m_Cap{ 0 };
	cv::UMat m_Image{};

	cv::Ptr<cv::face::Facemark> m_Facemark;

	cv::CascadeClassifier m_Cascade;

	std::vector<cv::Rect> m_Faces;
	std::vector<std::vector<cv::Point2f>> m_Fits;

};

