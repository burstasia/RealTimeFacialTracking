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


}

void AFaceAnimations::SetFacialExpression(const TArray<FVector2D>& currentTrackedPoints)
{


}

