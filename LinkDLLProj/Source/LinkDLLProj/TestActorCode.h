// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestActorCode.generated.h"

UCLASS()
class LINKDLLPROJ_API ATestActorCode : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestActorCode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
		USceneComponent* m_pRoot;

	UPROPERTY(EditAnywhere)
		USkeletalMeshComponent* m_pSkeletalMesh;

private:
	
	
};
