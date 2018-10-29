// Fill out your copyright notice in the Description page of Project Settings.

#include "FaceAnimations.h"
#include "components/SkeletalMeshComponent.h"
//#include "Runtime/Core/Public/GenericPlatform/GenericPlatformMath.h"

// Sets default values
AFaceAnimations::AFaceAnimations()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_pRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = m_pRoot;

	m_pSkeleton = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	m_pSkeleton->AttachTo(m_pRoot);
}

// Called when the game starts or when spawned
void AFaceAnimations::BeginPlay()
{
	Super::BeginPlay();

	m_FacialFeatureArray.Push(FFacialFeatureInfo{ "Smile_Lips_Opened_R_", 54, EExpressionEnum::Happy });
	m_FacialFeatureArray.Push(FFacialFeatureInfo{ "Smile_Lips_Opened_L_", 48, EExpressionEnum::Happy });
	m_FacialFeatureArray.Push(FFacialFeatureInfo{ "Mouth_Little_Opened", 8, EExpressionEnum::Surprised });
	m_FacialFeatureArray.Push(FFacialFeatureInfo{ "Eye_Closed_L", 37, EExpressionEnum::Closed });
	m_FacialFeatureArray.Push(FFacialFeatureInfo{ "Eye_Closed_R", 44, EExpressionEnum::Closed });
	
}

// Called every frame
void AFaceAnimations::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFaceAnimations::SetNeutralFace(const TArray<FVector2D>& trackedNeutral)
{
	m_NeutralFacePoints = trackedNeutral;
}

void AFaceAnimations::SetSmileFace(const TArray<FVector2D>& trackedSmile)
{
	m_SmileFacePoints = trackedSmile;
}

void AFaceAnimations::SetAngryFace(const TArray<FVector2D>& trackedAngry)
{
	m_AngryFacePoints = trackedAngry;
}

void AFaceAnimations::SetSurprisedFace(const TArray<FVector2D>& trackedSurprised)
{
	m_SurprisedFacePoints = trackedSurprised;
	m_LastFramePoints = trackedSurprised;

}

void AFaceAnimations::SetClosedEyes(const TArray<FVector2D>& trackedClosed)
{
	m_ClosedFacePoints = trackedClosed;
	m_LastFramePoints = trackedClosed;
}
void AFaceAnimations::SetMinMax()
{
	m_DistanceBetweenTemples = m_NeutralFacePoints[m_RightTemple].X - m_NeutralFacePoints[m_LeftTemple].X;

	for (int i = 0; i < m_FacialFeatureArray.Num(); i++)
	{
		FFacialFeatureInfo info = m_FacialFeatureArray[i];

		switch (info.expression)
		{
		case EExpressionEnum::Angry:
			MaxDistanceHelper(m_AngryFacePoints, info);
			break;

		case EExpressionEnum::Happy:
			MaxDistanceHelper(m_SmileFacePoints, info);
			break;

		case EExpressionEnum::Surprised:
			MaxDistanceHelper(m_SurprisedFacePoints, info);
			break;

		case EExpressionEnum::Closed:
			MaxDistanceHelper(m_ClosedFacePoints, info);
			break;
		}

		m_FacialFeatureArray[i] = info;
	}

}

void AFaceAnimations::SetFacialExpression(const TArray<FVector2D>& currentTrackedPoints)
{
	FVector2D neutralTranslation{};
	neutralTranslation = currentTrackedPoints[m_IndexMiddleFace] - m_NeutralFacePoints[m_IndexMiddleFace];

	//adjusting for person moving closer/further from the camera
	float currDistanceBetweenTemples = currentTrackedPoints[m_RightTemple].X - currentTrackedPoints[m_LeftTemple].X;
	float percentageDifference = (currDistanceBetweenTemples / m_DistanceBetweenTemples) - 1.0f;

	for (int i = 0; i < m_FacialFeatureArray.Num(); i++)
	{
		FFacialFeatureInfo info = m_FacialFeatureArray[i];

		//adjust the neutral point to the current position of the head
		info.neutralPos = m_NeutralFacePoints[info.indexFeature] + neutralTranslation;

		//calculate this distance
		FVector2D distance = currentTrackedPoints[info.indexFeature] - info.neutralPos;

		//only compare current distance between features if maxDistance has been adjusted
		float scaledMaxDistance = (info.maxDistance * percentageDifference) + info.maxDistance;
		//x or Y
		if (info.isY)
		{
			if ((currentTrackedPoints[info.indexFeature].Y - m_LastFramePoints[info.indexFeature].Y) > m_ThresholdMin && (currentTrackedPoints[info.indexFeature].Y - m_LastFramePoints[info.indexFeature].Y) < m_ThresholdMax)
			{
				float value = distance.Y / scaledMaxDistance;

				value = FMath::Abs(value);

				value = FMath::Clamp(value, 0.0f, 1.0f);

				m_pSkeleton->SetMorphTarget(info.morphTargetName, value);
			}
			
			else
			{
				distance = m_LastFramePoints[info.indexFeature] - info.neutralPos;

				float value = distance.Y / scaledMaxDistance;

				value = FMath::Abs(value);

				value = FMath::Clamp(value, 0.0f, 1.0f);

				m_pSkeleton->SetMorphTarget(info.morphTargetName, value);

			}
		}
		//compare it to the max distance

		//if the distance last frame compared to the distance this frame is lower than a certain threshold 
		//then we don't change the morph target

		//also if the distance is bigger than a certain threshold then we don't change it either

		//this way we aviod jittering and extreme movement because the face tracker bugs out occasionally
		//set the morphTaget
	}

	m_LastFramePoints = currentTrackedPoints;
}

void AFaceAnimations::MaxDistanceHelper(const TArray<FVector2D>& expressionPoints, FFacialFeatureInfo & info)
{
	FVector2D neutralTranslation{}; 
	FVector2D distance{};

	neutralTranslation = expressionPoints[m_IndexMiddleFace] - m_NeutralFacePoints[m_IndexMiddleFace]; //get the vector representing the distance between the eyebrows 
	info.neutralPos = m_NeutralFacePoints[info.indexFeature] + neutralTranslation;

	distance = expressionPoints[info.indexFeature] - info.neutralPos;

	if (info.isY)
	{
		info.maxDistance = distance.Y;
	}
	else
	{
		info.maxDistance = distance.X;
	}
}

