#include "stdafx.h"
#include "FaceTracker.h"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/face.hpp"

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
	//TOD: relative paths
	m_Facemark = createFacemarkLBF();
	m_Facemark->loadModel("D:/DAE/5thSemester/GradWork/Week_03/LinkDLLProj/Plugins/DLLs/lbfmodel.yaml");

	m_Cascade.load("D:/DAE/5thSemester/GradWork/Week_03/LinkDLLProj/Plugins/DLLs/haarcascade_frontalface_alt2.xml");

	//this threshold will determine if a point on the facial landmark should move
	m_Threshold = 0.1f;

	/*if (m_Cap.isOpened())
	{
		m_Cap.read(m_Image);

		m_Cascade.detectMultiScale(m_Image, m_Faces);

		bool ok = m_Facemark->fit(m_Image, m_Faces, m_Fits);
	}*/
}


FaceTracker::~FaceTracker()
{
}


void FaceTracker::Init()
{
}

void FaceTracker::GetLandmark(float * buf, int size)
{
	if (m_Cap.isOpened())
	{
		m_Cap.read(m_Image);

		m_Cascade.detectMultiScale(m_Image, m_Faces);

		//vector<vector<Point2f>> newFits;

		bool ok = m_Facemark->fit(m_Image, m_Faces, m_Fits);

		//TODO: fix magic numbers
		for (int j = 0, x = 0; j < 136; j +=2, x++ )
		{
			/*if (newFits[0][x].x - m_Fits[0][x].x > m_Threshold)
			{
				m_Fits[0][x].x = newFits[0][x].x;
			}
			if (newFits[0][x].y - m_Fits[0][x].y > m_Threshold)
			{
				m_Fits[0][x].y = newFits[0][x].y;
			}*/
			buf[j] = m_Fits[0][x].x;
			buf[j + 1] = m_Fits[0][x].y;
			circle(m_Image, m_Fits[0][x], 1, { 1.0f,0.0f,0.0f });
		}

		//m_Fits = newFits;
		imshow("F", m_Image);
	}
}

//float FaceTracker::GetLandmark(int index, bool isX)
//{
//	
//	if (m_Cap.isOpened())
//	{
//		m_Cap.read(m_Image);
//		
//		m_Cascade.detectMultiScale(m_Image, m_Faces);
//
//		bool ok = m_Facemark->fit(m_Image, m_Faces, m_Fits);
//
//		/*for (int i = 0; i < fits.size(); i++)
//		{
//			for (int j = 0; j < 68; j++)
//			{
//				circle(m_Image, fits[0][j], 3, Scalar(200, 0, 0), 2);
//			}
//		}
//		imshow("F", m_Image);*/
//
//		if (ok && m_Fits.size() > 0)
//		{
//			if (isX) return m_Fits[0][index].x;
//			else return m_Fits[0][index].y;
//		}
//	}
//	return 235834905.0f;
//}

