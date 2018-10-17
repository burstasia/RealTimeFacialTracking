#include "stdafx.h"
#include <vector>
#include "DLL.h"

DLL_Export void initFaceTracker()
{
	m_pFaceTracker = new FaceTracker{};
}

DLL_Export void returnTrackedPoints(float *buf, int size)
{
	
	m_pFaceTracker->GetLandmark(buf, size);
}


