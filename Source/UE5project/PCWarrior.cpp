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
	GetCharacterMovement()->MaxAcceleration = 2048.0f;
	GetCharacterMovement()->GroundFriction = 0.1f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2048.0f;
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

void APCWarrior::MoveSpeedToggle()
{
	Super::MoveSpeedToggle();
	if (!IsRun)
	{
		GetCharacterMovement()->MaxAcceleration = 0.0f;
		GetCharacterMovement()->BrakingDecelerationWalking = 1024.0f;
		GetWorldTimerManager().SetTimer(MoveSpeedTimerHandle, this, &APCWarrior::MoveSpeedTimer, 0.1f, true);
	}
	else
	{
		if(GetWorldTimerManager().IsTimerActive(MoveSpeedTimerHandle))
			GetWorldTimerManager().ClearTimer(MoveSpeedTimerHandle);
		GetCharacterMovement()->MaxAcceleration = 2048.0f;
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
		GetCharacterMovement()->BrakingDecelerationWalking = 2048.0f;
	}
}

void APCWarrior::MoveSpeedTimer()
{
	if (GetVelocity().Size() <= 300.0f)
	{
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;
		GetCharacterMovement()->MaxAcceleration = 2048.0f;
		GetWorldTimerManager().ClearTimer(MoveSpeedTimerHandle);
		GetCharacterMovement()->BrakingDecelerationWalking = 2048.0f;
	}
}