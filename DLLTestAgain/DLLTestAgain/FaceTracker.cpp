#include "stdafx.h"
#include "FaceTracker.h"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/face.hpp"

#include "PropertyManager.h"

#include <iostream>
#include <string>
#include <ctime>
#include <fstream>
#include <sstream>


using namespace std;
using namespace cv;
using namespace cv::face;



FaceTracker::FaceTracker()
{
	//relative path
	m_Facemark = createFacemarkLBF();

	std::string dllPath{ PropertyManager::path.begin() , PropertyManager::path.end() };// = PropertyManager::path.begin();

	int last{};
	last = dllPath.find_last_of('\\');
	dllPath = dllPath.substr(0,last);

	m_Facemark->loadModel(dllPath  + "\\lbfmodel.yaml");

	m_Cascade.load(dllPath + "\\haarcascade_frontalface_alt2.xml");

}

FaceTracker::~FaceTracker()
{
}

void FaceTracker::GetLandmark(float * buf, int size)
{
	if (m_Cap.isOpened())
	{
		m_Cap.read(m_Image);

		m_Cascade.detectMultiScale(m_Image, m_Faces);


		bool ok = m_Facemark->fit(m_Image, m_Faces, m_Fits);

		//TODO: fix magic numbers
		if (m_Fits.size() > 0)
		{
			for (int j = 0, x = 0; j < 136; j += 2, x++)
			{
				buf[j] = m_Fits[0][x].x;
				buf[j + 1] = m_Fits[0][x].y;
				circle(m_Image, m_Fits[0][x], 1, { 1.0f,0.0f,0.0f });
			}
		}
		imshow("F", m_Image);
	}
}


