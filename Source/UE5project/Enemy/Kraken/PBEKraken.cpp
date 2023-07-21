// Fill out your copyright notice in the Description page of Project Settings.


#include "PBEKraken.h"
#include "PBEKrakenAI.h"
//#include "../PBEnemyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

APBEKraken::APBEKraken()
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = APBEKrakenAI::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	//GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 90.0f);
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
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