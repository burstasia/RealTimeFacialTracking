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

	m_IsNeutral = true;
}

// Called when the game starts or when spawned
void AFaceAnimations::BeginPlay()
{
	Super::BeginPlay();

	m_FacialFeatureArray.Push(FFacialFeatureInfo{ "Smile_Lips_Opened_R_", 54, EExpressionEnum::Happy });
	//m_FacialFeatureArray.Push(FFacialFeatureInfo{ "Smile_Lips_Opened_L_", 48, EExpressionEnum::Happy });
	//m_FacialFeatureArray.Push(FFacialFeatureInfo{ "Mouth_Little_Opened", 8, EExpressionEnum::Surprised });
	//m_FacialFeatureArray.Push(FFacialFeatureInfo{ "Eye_Closed_L", 37, EExpressionEnum::Closed });
	//m_FacialFeatureArray.Push(FFacialFeatureInfo{ "Eye_Closed_R", 44, EExpressionEnum::Closed });
	
}

// Called every frame
void AFaceAnimations::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFaceAnimations::SetNeutralFace(const TArray<FVector2D>& trackedNeutral)
{
	m_NeutralFacePoints = trackedNeutral;
	m_DistanceBetweenTemples = FVector2D::Distance(m_NeutralFacePoints[m_RightTemple], m_NeutralFacePoints[m_LeftTemple]);
	TranslateFaceCoordinates(m_NeutralFacePoints);

}

void AFaceAnimations::SetSmileFace(const TArray<FVector2D>& trackedSmile)
{
	m_SmileFacePoints = trackedSmile;
	TranslateFaceCoordinates(m_SmileFacePoints);

}

void AFaceAnimations::SetAngryFace(const TArray<FVector2D>& trackedAngry)
{
	m_AngryFacePoints = trackedAngry;
	TranslateFaceCoordinates(m_AngryFacePoints);
}

void AFaceAnimations::SetSurprisedFace(const TArray<FVector2D>& trackedSurprised)
{
	m_SurprisedFacePoints = trackedSurprised;
	TranslateFaceCoordinates(m_SurprisedFacePoints);

}

void AFaceAnimations::SetClosedEyes(const TArray<FVector2D>& trackedClosed)
{
	m_ClosedFacePoints = trackedClosed;
	TranslateFaceCoordinates(m_ClosedFacePoints);
	m_LastFramePoints = trackedClosed;
}
void AFaceAnimations::SetMinMax()
{
	for (int i = 0; i < m_FacialFeatureArray.Num(); i++)
	{
		FFacialFeatureInfo info = m_FacialFeatureArray[i];

		switch (info.expression)
		{
		case EExpressionEnum::Angry:
			CalculateMaxDistance(m_AngryFacePoints, info);
			break;

		case EExpressionEnum::Happy:
			CalculateMaxDistance(m_SmileFacePoints, info);
			break;

		case EExpressionEnum::Surprised:
			CalculateMaxDistance(m_SurprisedFacePoints, info);
			break;

		case EExpressionEnum::Closed:
			CalculateMaxDistance(m_ClosedFacePoints, info);
			break;
		}

		m_FacialFeatureArray[i] = info;
	}

}

void AFaceAnimations::SetFacialExpression(const TArray<FVector2D>& currentTrackedPoints)
{
	for (int i = 0; i < m_FacialFeatureArray.Num(); i++)
	{
		FFacialFeatureInfo info = m_FacialFeatureArray[i];
		FVector2D currentPoint = currentTrackedPoints[info.indexFeature];

		TranslateFaceCoordinates(currentTrackedPoints, info.indexFeature, currentPoint);

		FVector2D neutralPoint = m_NeutralFacePoints[info.indexFeature];

		FVector2D distance = currentPoint - neutralPoint;

		if (info.isY)
		{
			float value = distance.Y / info.maxDistance;

			value = FMath::Abs(value);

			value = FMath::Clamp(value, 0.0f, 1.0f); //the most extreme of the face morph tends to look strange

			m_pSkeleton->SetMorphTarget(info.morphTargetName, value);

		}
		//get the current distance from neutral 
	}

}

void AFaceAnimations::CalculateMaxDistance(const TArray<FVector2D>& expressionPoints, FFacialFeatureInfo & info)
{
	FVector2D neutralTranslation{}; 
	FVector2D distance{};

	//neutralTranslation = expressionPoints[m_IndexMiddleFace] - m_NeutralFacePoints[m_IndexMiddleFace]; //get the vector representing the distance between the eyebrows 
	//info.neutralPos = m_NeutralFacePoints[info.indexFeature] + neutralTranslation;

	distance = expressionPoints[info.indexFeature] - m_NeutralFacePoints[info.indexFeature];

	if (info.isY)
	{
		info.maxDistance = distance.Y;
	}
	else
	{
		info.maxDistance = distance.X;
	}
}

void AFaceAnimations::TranslateFaceCoordinates(TArray<FVector2D>& currPoints)
{

	float currentDistance = FVector2D::Distance(currPoints[m_LeftTemple] , currPoints[m_RightTemple]);
	
	float scale = currentDistance / m_DistanceBetweenTemples;


	//constrct vector nose
	FVector tailNose = FVector{ 0, currPoints[30].X , currPoints[30].Y };
	FVector headNose = FVector{ 0, currPoints[27].X, currPoints[27].Y };
	FVector noseAxis = (headNose - tailNose);
	noseAxis = noseAxis.GetSafeNormal();

	FVector rightVector = FVector{ 0,1,0 };

	//angle in radians 0.1
	float angle = FMath::Acos(FVector::DotProduct(noseAxis, rightVector)) + (PI / 2.0f);

	FTransform translationComponent{ FVector{0 ,currPoints[30].X, currPoints[30].Y} };

	for (int i = 0; i < currPoints.Num(); i++)
	{
		auto newVector = translationComponent.SubtractTranslations(FTransform{ FVector{0.0f, currPoints[i].X, currPoints[i].Y} }, translationComponent);
		
		FVector2D translatedRotatedPoint{};
		translatedRotatedPoint.X = (newVector.Y  * FMath::Cos(angle)) - (newVector.Z * FMath::Sin(angle));
		translatedRotatedPoint.Y = (newVector.Z  * FMath::Cos(angle)) + (newVector.Y * FMath::Sin(angle));

		newVector.Y = translatedRotatedPoint.X;
		newVector.Z = translatedRotatedPoint.Y;

		newVector = FVector{0.0f, (newVector.Y * (1.0f - scale) + newVector.Y), (newVector.Z * (1.0f - scale) + newVector.Z )};

		//adjusting because points are always just a little bit off
		FVector2D distance{0.0f,0.0f};
		if (!m_IsNeutral)
		{
			FVector2D distance = currPoints[0] - m_NeutralFacePoints[0];
		}

		currPoints[i].X = newVector.Y + distance.X;
		currPoints[i].Y = newVector.Z + distance.Y;

		m_IsNeutral = false;
	}
}

void AFaceAnimations::TranslateFaceCoordinates(const TArray<FVector2D>& currPoints, int indexFeature, FVector2D & currentPoint)
{
	float currentDistance = FVector2D::Distance(currPoints[m_LeftTemple], currPoints[m_RightTemple]);

	float scale = currentDistance / m_DistanceBetweenTemples;


	//constrct vector nose
	FVector tailNose = FVector{ 0, currPoints[30].X , currPoints[30].Y };
	FVector headNose = FVector{ 0, currPoints[27].X, currPoints[27].Y };
	FVector noseAxis = (headNose - tailNose);
	noseAxis = noseAxis.GetSafeNormal();

	FVector rightVector = FVector{ 0,1,0 };

	//angle in radians 0.1
	float angle = FMath::Acos(FVector::DotProduct(noseAxis, rightVector)) + (PI / 2.0f);

	FTransform translationComponent{ FVector{ 0 ,currPoints[30].X, currPoints[30].Y } };


	auto newVector = translationComponent.SubtractTranslations(FTransform{ FVector{ 0.0f, currPoints[indexFeature].X, currPoints[indexFeature].Y } }, translationComponent);

	FVector2D translatedRotatedPoint{};
	translatedRotatedPoint.X = (newVector.Y  * FMath::Cos(angle)) - (newVector.Z * FMath::Sin(angle));
	translatedRotatedPoint.Y = (newVector.Z  * FMath::Cos(angle)) + (newVector.Y * FMath::Sin(angle));

	newVector.Y = translatedRotatedPoint.X;
	newVector.Z = translatedRotatedPoint.Y;

	newVector = FVector{ 0.0f, (newVector.Y * (1.0f - scale) + newVector.Y), (newVector.Z * (1.0f - scale) + newVector.Z) };

	//adjusting because points are always just a little bit off
	FVector2D distance{ 0.0f,0.0f };
	if (!m_IsNeutral)
	{
		FVector2D distance = currPoints[0] - m_NeutralFacePoints[0];
	}

	currPoints[i].X = newVector.Y + distance.X;
	currPoints[i].Y = newVector.Z + distance.Y;

	m_IsNeutral = false;
	
}
