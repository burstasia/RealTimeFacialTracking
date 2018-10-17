#pragma once
#include "stdafx.h"
#include <vector>
#define DLL_Export __declspec(dllexport)

#include "FaceTracker.h"

extern "C"
{
	DLL_Export void _cdecl initFaceTracker();
	//DLL_Export  void _cdecl returnTrackedPoints(char *buf, int count);
	DLL_Export  void _cdecl returnTrackedPoints(float *buf, int size);
}

FaceTracker* m_pFaceTracker;
