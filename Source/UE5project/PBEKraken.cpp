// Fill out your copyright notice in the Description page of Project Settings.


#include "PBEKraken.h"
#include "PBEKrakenAI.h"

APBEKraken::APBEKraken()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>PBEK_Mannequin(TEXT("/Game/Kraken/Meshes/KRAKEN.KRAKEN"));
	if (PBEK_Mannequin.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(PBEK_Mannequin.Object);
	}

	AIControllerClass = APBEKrakenAI::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void APBEKraken::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APBEKraken::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}