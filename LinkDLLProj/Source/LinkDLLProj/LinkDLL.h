// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LinkDLL.generated.h"

/**
 * 
 */
UCLASS()
class LINKDLLPROJ_API ULinkDLL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	explicit ULinkDLL();
	~ULinkDLL();

	UFUNCTION(BlueprintCallable, Category = "My DLL Library") static bool importDLL(FString folder, FString name);
	
	UFUNCTION(BlueprintCallable, Category = "My DLL Library") static bool importMethodInitFaceTracker();
	UFUNCTION(BlueprintCallable, Category = "My DLL Library") static void initFaceTracker();

	UFUNCTION(BlueprintCallable, Category = "My DLL Library") static bool importMethodReturnTrackedPoints();
	
	UFUNCTION(BlueprintCallable, Category = "My DLL Library") static void returnTrackedPointsFromDLLArray(TArray<FVector2D>& pointsReturn);

	static float m_pPointArray[136];
	//UFUNCTION(BlueprintCallable, Category = "My DLL Library") static void returnTrackedPointsFromDLL(char *buf, int count, TArray<float>& points);
};
