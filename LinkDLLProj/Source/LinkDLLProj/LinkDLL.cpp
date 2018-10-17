// Fill out your copyright notice in the Description page of Project Settings.

#include "LinkDLL.h"

//typedef void(*_returnTrackedPoints)(char *buf, int count); //create a method to store the DLL method returnTrackedPoints

typedef void(*_initFaceTracker)();
_initFaceTracker m_initFaceTrackerFromDLL;

//typedef float(*_returnTrackedPoints)(int index, bool isX);
//_returnTrackedPoints m_returnTrackedPointsFromDll;

typedef void(*_returnTrackedPoints)(float *buf, int size);
_returnTrackedPoints m_returnTrackedPointsFromDll;

void *v_dllHandle;

float ULinkDLL::m_pPointArray[136]{};

#pragma region Load DLL

ULinkDLL::ULinkDLL()
{
}

ULinkDLL::~ULinkDLL()
{
}

bool ULinkDLL::importDLL(FString folder, FString name)
{
	FString filePath = *FPaths::ProjectPluginsDir() + folder + "/" + name;

	if (FPaths::FileExists(filePath))
	{
		v_dllHandle = FPlatformProcess::GetDllHandle(*filePath); // retrieve the DLL

		if (v_dllHandle != NULL)
		{
			return  true;
		}
	}
	return false;
}

UFUNCTION(BlueprintCallable, Category = "My DLL Library") bool ULinkDLL::importMethodInitFaceTracker()
{
	if (v_dllHandle != NULL)
	{
		m_initFaceTrackerFromDLL = NULL;
		FString procName = "initFaceTracker"; // needs to be exactly the same as the DLL method

		m_initFaceTrackerFromDLL = (_initFaceTracker)FPlatformProcess::GetDllExport(v_dllHandle, *procName);

		if (m_initFaceTrackerFromDLL != NULL)
		{
			return true;
		}
	}

	return false;
}

UFUNCTION(BlueprintCallable, Category = "My DLL Library") void ULinkDLL::initFaceTracker()
{
	if (m_initFaceTrackerFromDLL != NULL)
	{
		m_initFaceTrackerFromDLL();
	}
}


#pragma endregion Load DLL

#pragma region Import Methods
bool ULinkDLL::importMethodReturnTrackedPoints()
{
	if (v_dllHandle != NULL)
	{
		m_returnTrackedPointsFromDll = NULL;
		FString procName = "returnTrackedPoints"; // needs to be exactly the same as the DLL method

		m_returnTrackedPointsFromDll = (_returnTrackedPoints)FPlatformProcess::GetDllExport(v_dllHandle, *procName);

		if (m_returnTrackedPointsFromDll != NULL)
		{
			return true;
		}
	}

	return false;
}
#pragma endregion Import Methods

#pragma region Method Calls
//void ULinkDLL::returnTrackedPointsFromDLL(char * buf, int count, TArray<float>& points)
//{
//	if (m_returnTrackedPointsFromDll != NULL)
//	{
//		m_returnTrackedPointsFromDll(buf, count);
//
//	}
//}
//float ULinkDLL::returnTrackedPointsFromDLL()
//{
//	if (m_returnTrackedPointsFromDll != NULL)
//	{
//		float out = m_returnTrackedPointsFromDll();
//		return out;
//	}
//	else return 314351.0f;
//}

//void ULinkDLL::returnTrackedPointsFromDLLArray(TArray<FVector2D> points, TArray<FVector2D>& pointsReturn)
//{
//	pointsReturn = points;
//
//
//	if (m_returnTrackedPointsFromDll != NULL)
//	{
//		for (int i = 0; i < pointsReturn.Max(); i++)
//		{
//			pointsReturn[i].X = m_returnTrackedPointsFromDll(i, true);
//			pointsReturn[i].Y = m_returnTrackedPointsFromDll(i, false);
//		}
//	}
//}

void ULinkDLL::returnTrackedPointsFromDLLArray(TArray<FVector2D>& pointsReturn)
{
	//pointsReturn = points;
	if (pointsReturn.Max() == 0)
	{
		for (size_t i = 0; i < 68; i++)
		{
			pointsReturn.Push({ 0.0f, 0.0f });
		}
	}
	if (m_returnTrackedPointsFromDll != NULL)
	{
		//return value
		m_returnTrackedPointsFromDll(m_pPointArray, 136);
		for (size_t i = 0, x = 0; i < 136; i+=2, x++)
		{
			auto newFacePoint = FVector2D(m_pPointArray[i], m_pPointArray[i+1]);
			pointsReturn[x] = newFacePoint;// .Push(newFacePoint);
		}
	}
}
#pragma endregion Method Calls

