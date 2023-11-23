// Fill out your copyright notice in the Description page of Project Settings.


#include "PBEHAnimInstance.h"
#include "PBEHuman.h"
#include "GameFramework/PawnMovementComponent.h"

UPBEHAnimInstance::UPBEHAnimInstance()
{
	
}

void UPBEHAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Character = Cast<APBEHuman>(TryGetPawnOwner());

}

void UPBEHAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (Character)
	{
		Speed = Character->GetVelocity().Size();
		Direction = CalculateDirection(Character->GetVelocity(), Character->GetActorRotation());
		IsDead = Character->GetHealth_Implementation() < 0.0f;
		IsInAir = Character->GetMovementComponent()->IsFalling();
	}
}

void UPBEHAnimInstance::PlayMontage(MontageType Type)
{
	switch (Type)
	{
	case MontageType::Attack:
		Montage_Play(AttackMontage, 1.0f);
		break;
	case MontageType::Death:
		Montage_Play(DeathMontage, 1.0f);
		IsDead = true;
		break;
	case MontageType::Hit:
		Montage_Play(HitMontage, 1.0f);
		break;
	default:
		break;
	}
}

MontageType UPBEHAnimInstance::CheckMontage(UAnimMontage* Montage)
{
	if (Montage == AttackMontage)
		return MontageType::Attack;
	else if (Montage == DeathMontage)
		return MontageType::Death;
	else if (Montage == HitMontage)
		return MontageType::Hit;
	else if (Montage == AppearMontage)
		return MontageType::Appear;
	else
		return MontageType::None;
}

void UPBEHAnimInstance::AnimNotify_NOT_StartHit()
{
	OnStartHit.Broadcast();
}

void UPBEHAnimInstance::AnimNotify_NOT_EndHit()
{
	OnEndHit.Broadcast();
}