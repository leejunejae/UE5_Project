// Fill out your copyright notice in the Description page of Project Settings.


#include "PBEHuman.h"
#include "PBEHumanAI.h"
#include "GameFramework/CharacterMovementComponent.h"

APBEHuman::APBEHuman()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	AIControllerClass = APBEHumanAI::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
}