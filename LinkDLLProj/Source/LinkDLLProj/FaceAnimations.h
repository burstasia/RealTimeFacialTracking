// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FaceAnimations.generated.h"
UENUM()

enum ExpressionEnum
{
	Surprised = 0,
	Angry = 1,
	Happy = 2
};

USTRUCT()
struct FFacialFeatureInfo
{
	GENERATED_BODY()
		UPROPERTY()
			float maxDistance;
		UPROPERTY()
			FVector2D neutralPos;
		UPROPERTY()
			bool isY;
		UPROPERTY()
			FName morphTargetName;
		UPROPERTY()
			int indexFeature;
		UPROPERTY()
			ExpressionEnum expression;

		FFacialFeatureInfo(const FName& morphName, int indexFeature, ExpressionEnum expressionFeature) :
			maxDistance(0.0f),
			neutralPos(FVector2D{ 0.0f,0.0f }),
			isY(true),
			morphTargetName(morphName),
			indexFeature(indexFeature),
			expression(expressionFeature)
		{}
};



UCLASS()
class LINKDLLPROJ_API AFaceAnimations : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFaceAnimations();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Calibration") void GetNeutralFace(const TArray<FVector2D>& trackedNeutral);
	UFUNCTION(BlueprintCallable, Category = "Calibration") void GetSmileFace(const TArray<FVector2D>& trackedSmile);
	UFUNCTION(BlueprintCallable, Category = "Calibration") void GetAngryFace(const TArray<FVector2D>& trackedAngry);
	UFUNCTION(BlueprintCallable, Category = "Calibration") void GetSurprisedFace(const TArray<FVector2D>& trackedSurprised);

	UFUNCTION(BlueprintCallable, Category = "Calibration") void SetMinMax();

	UFUNCTION(BlueprintCallable, Category = "Expression") void SetFacialExpression(const TArray<FVector2D>& currentTrackedPoints);

	UPROPERTY()
		TArray<FVector2D> m_NeutralFacePoints;
	UPROPERTY()
		TArray<FVector2D> m_SmileFacePoints;
	UPROPERTY()
		TArray<FVector2D> m_AngryFacePoints;
	UPROPERTY()
		TArray<FVector2D> m_SurprisedFacePoints;

	UPROPERTY()
		TArray<FFacialFeatureInfo> m_FacialFeatureArray;

};
