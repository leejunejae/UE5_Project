// Fill out your copyright notice in the Description page of Project Settings.


#include "PCWarrior.h"
#include "PCWAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

APCWarrior::APCWarrior()
{
	PrimaryActorTick.bCanEverTick = true;


	static ConstructorHelpers::FObjectFinder<USkeletalMesh>PBC_Mannequin(TEXT("/Game/Character/C_Mesh/CM_Warrior/Paladin_WProp_J_Nordstrom.Paladin_WProp_J_Nordstrom"));
	if (PBC_Mannequin.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(PBC_Mannequin.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance>WARRIOR_ANIM(TEXT("/Game/Character/C_Animation/CA_Warrior/CAW_BP/CAW_AnimBlueprint.CAW_AnimBlueprint_C"));
	if (WARRIOR_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(WARRIOR_ANIM.Class);
	}

	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
}

void APCWarrior::BeginPlay()
{
	Super::BeginPlay();
}

void APCWarrior::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APCWarrior::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}