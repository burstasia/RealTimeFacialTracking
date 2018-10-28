// Fill out your copyright notice in the Description page of Project Settings.

#include "FaceAnimations.h"


// Sets default values
AFaceAnimations::AFaceAnimations()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFaceAnimations::BeginPlay()
{
	Super::BeginPlay();

	m_FacialFeatureArray.Push(FFacialFeatureInfo{ "smileRight", 55, EExpressionEnum::Happy });

	
}

// Called every frame
void AFaceAnimations::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFaceAnimations::GetNeutralFace(const TArray<FVector2D>& trackedNeutral)
{
	m_NeutralFacePoints = trackedNeutral;
}

void AFaceAnimations::GetSmileFace(const TArray<FVector2D>& trackedSmile)
{
	m_SmileFacePoints = trackedSmile;
}

void AFaceAnimations::GetAngryFace(const TArray<FVector2D>& trackedAngry)
{
	m_AngryFacePoints = trackedAngry;
}

void AFaceAnimations::GetSurprisedFace(const TArray<FVector2D>& trackedSurprised)
{
	m_SurprisedFacePoints = trackedSurprised;
}

void AFaceAnimations::SetMinMax()
{
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
		}

		m_FacialFeatureArray[i] = info;
	}

}

void AFaceAnimations::SetFacialExpression(const TArray<FVector2D>& currentTrackedPoints)
{


}

void AFaceAnimations::MaxDistanceHelper(const TArray<FVector2D>& expressionPoints, FFacialFeatureInfo & info)
{
	FVector2D neutralTranslation{}; 
	FVector2D distance{};

	neutralTranslation = m_NeutralFacePoints[m_IndexMiddleFace] - expressionPoints[m_IndexMiddleFace]; //get the vector representing the distance between the eyebrows 
	info.neutralPos = m_NeutralFacePoints[info.indexFeature] + neutralTranslation;

	distance = info.neutralPos - expressionPoints[info.indexFeature];

	if (info.isY)
	{
		info.maxDistance = distance.Y;
	}
	else
	{
		info.maxDistance = distance.X;
	}
}

