// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FaceAnimations.generated.h"


UENUM()
namespace EExpressionEnum
{
	enum ExpressionTypes
	{
		Surprised = 0,
		Angry = 1,
		Happy = 2,
		Closed = 3,
		Pucker = 4
	};
}


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
			TEnumAsByte<EExpressionEnum::ExpressionTypes> expression;
		UPROPERTY()
			bool compareOtherIndex;
		UPROPERTY()
			FVector2D indexFeatureCompare;
		UPROPERTY()
			bool isOpen;

		FFacialFeatureInfo()
		{}
		FFacialFeatureInfo(const FName& morphName, int indexFeature, TEnumAsByte<EExpressionEnum::ExpressionTypes> expressionFeature, bool compare, FVector2D indexOther, bool isOpen) :
			maxDistance(0.0f),
			neutralPos(FVector2D{ 0.0f,0.0f }),
			isY(true),
			morphTargetName(morphName),
			indexFeature(indexFeature),
			expression(expressionFeature),
			compareOtherIndex(compare),
			indexFeatureCompare(indexOther),
			isOpen(isOpen)
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

	UFUNCTION(BlueprintCallable, Category = "Calibration") void SetNeutralFace(const TArray<FVector2D>& trackedNeutral);
	UFUNCTION(BlueprintCallable, Category = "Calibration") void SetSmileFace(const TArray<FVector2D>& trackedSmile);
	UFUNCTION(BlueprintCallable, Category = "Calibration") void SetAngryFace(const TArray<FVector2D>& trackedAngry);
	UFUNCTION(BlueprintCallable, Category = "Calibration") void SetSurprisedFace(const TArray<FVector2D>& trackedSurprised);
	UFUNCTION(BlueprintCallable, Category = "Calibration") void SetClosedEyes(const TArray<FVector2D>& trackedClosed);
	UFUNCTION(BlueprintCallable, Category = "Calibration") void SetPucker(const TArray<FVector2D>& trackedPucker);

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
		TArray<FVector2D> m_ClosedFacePoints;
	UPROPERTY()
		TArray<FVector2D> m_PuckerFacePoints;

	UPROPERTY()
		TArray<FFacialFeatureInfo> m_FacialFeatureArray;
	UPROPERTY()
		TArray<FVector2D> m_LastFramePoints;


	UPROPERTY()
		USceneComponent* m_pRoot;

	UPROPERTY(EditAnywhere)
		USkeletalMeshComponent* m_pSkeleton;

	int m_IndexMiddleFace{ 28 };

	//might've gotten these confused
	int m_LeftTemple{0};
	int m_RightTemple{16};
	float m_DistanceBetweenTemples{};

	//these values were tweaked until the best solution was found
	float m_ThresholdMin{ 0.8f };
	float m_ThresholdMax{ 4.0f };

private:
	void CalculateMaxDistance(const TArray<FVector2D>& expressionPoints, FFacialFeatureInfo& info);
	void TranslateFaceCoordinates(TArray<FVector2D>& currPoints);
	void TranslateFaceCoordinates(const TArray<FVector2D>& currPoints, int indexFeature, FVector2D& currentPoint);
	void SetValue(const TArray<FVector2D>& currPoints, FFacialFeatureInfo& info);
	bool m_IsNeutral;
};
