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

	m_FacialFeatureArray.Push(FFacialFeatureInfo{ "Smile_Lips_Opened_R_", 54, EExpressionEnum::Happy, true,  {62.0f, 66.0f}, true });
	m_FacialFeatureArray.Push(FFacialFeatureInfo{ "Smile_Lips_Opened_L_", 48, EExpressionEnum::Happy, true,  { 62.0f, 66.0f }, true });
	m_FacialFeatureArray.Push(FFacialFeatureInfo{ "Smile_Lips_Closed_R_", 54, EExpressionEnum::Happy, true,{ 62.0f, 66.0f }, false });
	m_FacialFeatureArray.Push(FFacialFeatureInfo{ "Smile_Lips_Closed_L_", 48, EExpressionEnum::Happy, true,{ 62.0f, 66.0f }, false });
	
	m_FacialFeatureArray.Push(FFacialFeatureInfo{ "Mouth_Little_Opened", 8, EExpressionEnum::Surprised, true,{ 62.0f, 66.0f }, true });
	m_FacialFeatureArray.Push(FFacialFeatureInfo{ "Nostrils_Sneer_L_", 22, EExpressionEnum::Angry, false, {0.0f,0.0f}, false });
	m_FacialFeatureArray.Push(FFacialFeatureInfo{ "Nostrils_Sneer_R_", 21, EExpressionEnum::Angry, false,{ 0.0f,0.0f }, false });
	m_FacialFeatureArray.Push(FFacialFeatureInfo{ "Eyebrow_Raised_R_", 24, EExpressionEnum::Surprised, false,{ 0.0f,0.0f }, false });
	m_FacialFeatureArray.Push(FFacialFeatureInfo{ "Eyebrow_Raised_L_", 19, EExpressionEnum::Surprised, false,{ 0.0f,0.0f }, false });
	//m_FacialFeatureArray.Push(FFacialFeatureInfo{ "Eye_Closed_L", 37, EExpressionEnum::Closed, false, {0.0f,0.0f}, false });
	//m_FacialFeatureArray.Push(FFacialFeatureInfo{ "Eye_Closed_R", 44, EExpressionEnum::Closed, false,{ 0.0f,0.0f }, false });

	//m_FacialFeatureArray.Push(FFacialFeatureInfo{ "Frown_R_", 54, EExpressionEnum::Angry, true, {62.0f, 66.0f}, false });
	//m_FacialFeatureArray.Push(FFacialFeatureInfo{ "Frown_L_", 48, EExpressionEnum::Angry, true,{ 62.0f, 66.0f }, false });
	//m_FacialFeatureArray.Push(FFacialFeatureInfo{ "Kiss_R_", 54, EExpressionEnum::Pucker, true,{ 62.0f, 66.0f }, false });
	//m_FacialFeatureArray[4].isY = false;
	//m_FacialFeatureArray.Push(FFacialFeatureInfo{ "Kiss_L_", 48, EExpressionEnum::Pucker, true,{ 62.0f, 66.0f }, false });
	//m_FacialFeatureArray[5].isY = false;
	
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
	m_LastFramePoints = m_SmileFacePoints;

}

void AFaceAnimations::SetAngryFace(const TArray<FVector2D>& trackedAngry)
{
	m_AngryFacePoints = trackedAngry;
	TranslateFaceCoordinates(m_AngryFacePoints);
	m_LastFramePoints = m_AngryFacePoints;
}

void AFaceAnimations::SetSurprisedFace(const TArray<FVector2D>& trackedSurprised)
{
	m_SurprisedFacePoints = trackedSurprised;
	TranslateFaceCoordinates(m_SurprisedFacePoints);
	m_LastFramePoints = m_SurprisedFacePoints;
}

void AFaceAnimations::SetClosedEyes(const TArray<FVector2D>& trackedClosed)
{
	m_ClosedFacePoints = trackedClosed;
	TranslateFaceCoordinates(m_ClosedFacePoints);
	m_LastFramePoints = m_ClosedFacePoints;
}

void AFaceAnimations::SetPucker(const TArray<FVector2D>& trackedPucker)
{
	m_PuckerFacePoints = trackedPucker;
	TranslateFaceCoordinates(m_PuckerFacePoints);
	m_LastFramePoints = m_PuckerFacePoints;
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
		case EExpressionEnum::Pucker:
			CalculateMaxDistance(m_PuckerFacePoints, info);
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

		float distance = FVector2D::Distance(m_LastFramePoints[info.indexFeature], currentTrackedPoints[info.indexFeature]);
		if (distance > m_ThresholdMin && distance < m_ThresholdMax)
		{
			if (info.compareOtherIndex)
			{
				//compare the distances of the compared indexes
				FVector2D point1 = currentTrackedPoints[int(info.indexFeatureCompare.X)];
				FVector2D point2 = currentTrackedPoints[int(info.indexFeatureCompare.X)];

				TranslateFaceCoordinates(currentTrackedPoints, int(info.indexFeatureCompare.X), point1);
				TranslateFaceCoordinates(currentTrackedPoints, int(info.indexFeatureCompare.Y), point2);

				float distance = FMath::Abs(FVector2D::Distance(point1, point2));

				//if the following conditions are not met then don't set the values at all
				//so if we have a morph target that has an open smile, and the mouth isn't open, then the value shouldn't be set
				if (info.isOpen && distance > 3.0f)
				{
					SetValue(currentTrackedPoints, info);
				}
				else if (!info.isOpen && distance < 3.0f)
				{
					SetValue(currentTrackedPoints, info);
				}
				else
				{
					m_pSkeleton->SetMorphTarget(info.morphTargetName, 0.0f);
				}
			}
			else
			{
				SetValue(currentTrackedPoints, info);
			}
		}
		
	}
	m_LastFramePoints = currentTrackedPoints;
	
}

void AFaceAnimations::CalculateMaxDistance(const TArray<FVector2D>& expressionPoints, FFacialFeatureInfo & info)
{
	FVector2D neutralTranslation{}; 
	FVector2D distance{};

	//neutralTranslation = expressionPoints[m_IndexMiddleFace] - m_NeutralFacePoints[m_IndexMiddleFace]; //get the vector representing the distance between the eyebrows 
	//info.neutralPos = m_NeutralFacePoints[info.indexFeature] + neutralTranslation;

	distance = (expressionPoints[info.indexFeature] - m_NeutralFacePoints[info.indexFeature]) * 1.1f;

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

	currentPoint.X = newVector.Y + distance.X;
	currentPoint.Y = newVector.Z + distance.Y;

	m_IsNeutral = false;
	
}

void AFaceAnimations::SetValue(const TArray<FVector2D>& currPoints, FFacialFeatureInfo& info)
{
	FVector2D currentPoint = currPoints[info.indexFeature];

	TranslateFaceCoordinates(currPoints, info.indexFeature, currentPoint);


	FVector2D neutralPoint = m_NeutralFacePoints[info.indexFeature];

	FVector2D distance = currentPoint - neutralPoint;

	if (info.isY)
	{

		if (info.maxDistance < 0.0f && distance.Y < 0.0f)
		{
			float value = distance.Y / info.maxDistance;

			value = FMath::Abs(value);

			value = FMath::Clamp(value, 0.0f, 1.0f);

			m_pSkeleton->SetMorphTarget(info.morphTargetName, value);
		}
		else if (info.maxDistance > 0.0f && distance.Y > 0.0f)
		{
			float value = distance.Y / info.maxDistance;

			value = FMath::Abs(value);

			value = FMath::Clamp(value, 0.0f, 1.0f);

			m_pSkeleton->SetMorphTarget(info.morphTargetName, value);
		}

	}
	else
	{
		if (info.maxDistance < 0.0f && distance.X < 0.0f)
		{
			float value = distance.X / info.maxDistance;

			value = FMath::Abs(value);

			value = FMath::Clamp(value, 0.0f, 1.0f);

			m_pSkeleton->SetMorphTarget(info.morphTargetName, value);
		}
		else if (info.maxDistance > 0.0f && distance.X > 0.0f)
		{
			float value = distance.X / info.maxDistance;

			value = FMath::Abs(value);

			value = FMath::Clamp(value, 0.0f, 1.0f);

			m_pSkeleton->SetMorphTarget(info.morphTargetName, value);
		}
	}

}
