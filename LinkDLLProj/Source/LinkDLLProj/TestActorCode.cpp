// Fill out your copyright notice in the Description page of Project Settings.

#include "TestActorCode.h"
#include "Components/SkeletalMeshComponent.h"
#include "LinkDLL.h"

// Sets default values
ATestActorCode::ATestActorCode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_pRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = m_pRoot;

	m_pSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	m_pSkeletalMesh->AttachTo(m_pRoot);
}

// Called when the game starts or when spawned
void ATestActorCode::BeginPlay()
{
	Super::BeginPlay();
	
	ImportDLL}

// Called every frame
void ATestActorCode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

