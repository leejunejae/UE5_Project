// Fill out your copyright notice in the Description page of Project Settings.


#include "PCWAnimInstance.h"
#include "PCWarrior.h"
#include "GameFramework/PawnMovementComponent.h"

UPCWAnimInstance::UPCWAnimInstance()
{
	CurrentSpeed = 0.0f;
	IsInAir = false;
}

void UPCWAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		CurrentSpeed = Pawn->GetVelocity().Size();
		auto Character = Cast<APCWarrior>(Pawn);
		if (Character)
		{
			Speed = Character->GetVelocity().Size();
			Direction = (Warrior_AnimInstance->CalculateDirection(Character->GetVelocity(), Character->GetActorRotation()));
			IsInAir = Character->GetMovementComponent()->IsFalling();
			Pitch = Character->GetBaseAimRotation().Pitch;
			Yaw = Character->GetBaseAimRotation().Yaw;
		}
	}
}