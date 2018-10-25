// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FaceAnimations.generated.h"

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
	
	UPROPERTY()
		TArray<FVector2D> m_NeutralFacePoints;
	UPROPERTY()
		TArray<FVector2D> m_SmileFacePoints;
	UPROPERTY()
		TArray<FVector2D> m_AngryFacePoints;
	UPROPERTY()
		TArray<FVector2D> m_SurprisedFacePoints;
};
