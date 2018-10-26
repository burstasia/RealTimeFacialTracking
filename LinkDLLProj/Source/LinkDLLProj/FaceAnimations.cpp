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

	m_FacialFeatureArray.Push(FFacialFeatureInfo{ "smileRight", 55, ExpressionEnum::Happy });

	
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
	FVector2D neutralTranslation{}; //28

	for (int i = 0; i < m_FacialFeatureArray.Num(); i++)
	{
		FFacialFeatureInfo info = m_FacialFeatureArray[i];

		switch (info.expression)
		{
		case ExpressionEnum::Angry:
			//set neutral pos
			//set max distance - adjust current pos of index with neutralTranslation
			info.neutralPos = m_NeutralFacePoints[28];
			neutralTranslation = Size(m_NeutralFacePoints[info.indexFeature] - m_AngryFacePoints[info.indexFeature]);
			info.maxDistance = 
			break;

		case ExpressionEnum::Happy:
			break;

		case ExpressionEnum::Surprised:
			break;
		}
	}

}

void AFaceAnimations::SetFacialExpression(const TArray<FVector2D>& currentTrackedPoints)
{


}

