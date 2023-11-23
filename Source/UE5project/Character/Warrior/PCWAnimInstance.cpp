// Fill out your copyright notice in the Description page of Project Settings.


#include "PCWAnimInstance.h"
#include "PCWarrior.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PawnMovementComponent.h"

UPCWAnimInstance::UPCWAnimInstance()
{
	CurrentSpeed = 0.0f;
	IsInAir = false;
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/Character/C_Warrior/CW_Animation/CWA_BP/CWAB_AttackMontage.CWAB_AttackMontage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DEATH_MONTAGE(TEXT("/Game/Character/C_Warrior/CW_Animation/CWA_BP/CWAB_DeathMontage.CWAB_DeathMontage"));
	if (DEATH_MONTAGE.Succeeded())
	{
		DeathMontage = DEATH_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DODGE_MONTAGE(TEXT("/Game/Character/C_Warrior/CW_Animation/CWA_BP/CWAB_DodgeMontage.CWAB_DodgeMontage"));
	if (DODGE_MONTAGE.Succeeded())
	{
		DodgeMontage = DODGE_MONTAGE.Object;
	}
}

void UPCWAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Character = Cast<APCWarrior>(TryGetPawnOwner());
}

void UPCWAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (Character)
	{
		Speed = Character->GetVelocity().Size();
		CombatMode = Character->ReturnReadiness();
		Direction = CalculateDirection(Character->GetVelocity(), Character->GetActorRotation());
		IsInAir = Character->GetMovementComponent()->IsFalling();
		IsAttack = Character->IsAttacking();
		IsHeavyAttack = Character->CheckHeavyAttack();
		ComboCount = Character->CheckCombo();
		SetPitch();
		SetRootYawOffset();
	}
}

void UPCWAnimInstance::SetPitch()
{
	FRotator PawnRotation = Character->GetActorRotation();
	FRotator AimRotation = Character->GetBaseAimRotation();
	Pitch = UKismetMathLibrary::NormalizedDeltaRotator(AimRotation, PawnRotation).Pitch;
}

void UPCWAnimInstance::SetRootYawOffset()
{
	if (Speed > 0.f || IsAnyMontagePlaying())
	{
		IsMove = true;
		RootYawOffset = 0.0f;
		return;
	}

	YawLastTick = Yaw;
	Yaw = Character->GetActorRotation().Yaw;
	if (IsMove)
	{
		YawLastTick = Yaw;
		IsMove = false;
	}
	YawChangeOverFrame = YawLastTick - Yaw;

	RootYawOffset = UKismetMathLibrary::NormalizeAxis(YawChangeOverFrame + RootYawOffset);

	if (GetCurveValue(Turning) > 0.0f)
	{
		DistanceCurveValueLastFrame = DistanceCurveValue;
		DistanceCurveValue = GetCurveValue(DistanceCurve);

		(RootYawOffset > 0.0f) ? TurnDirection = -1.0f : TurnDirection = 1.0f;
		DistanceCurveDifference = DistanceCurveValueLastFrame - DistanceCurveValue;

		RootYawOffset = RootYawOffset - (DistanceCurveDifference * TurnDirection);
		ABSRootYawOffset = UKismetMathLibrary::Abs(RootYawOffset);
		if (ABSRootYawOffset > MaxTurnAngle)
		{
			YawToSubtract = ABSRootYawOffset - MaxTurnAngle;
			YawMultiplier = 0.0f;
			(RootYawOffset > 0.0f) ? YawMultiplier = 1.0f : YawMultiplier = -1.0f;
			YawToSubtract = YawToSubtract * YawMultiplier;

			RootYawOffset = RootYawOffset - YawToSubtract;
		}
	}

	return;
}


void UPCWAnimInstance::PlayMontage(MontageType Type, int32 Section)
{
	switch (Type)
	{
	case MontageType::Attack:
		//Montage_Play(AttackMontage, 1.0f);
		break;
	case MontageType::Death:
		Montage_Play(DeathMontage, 1.0f);
		break;
	case MontageType::Dodge:
		Montage_Play(DodgeMontage, 1.0f);
		break;
	default:
		break;
	}
}

void UPCWAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage);
}

void UPCWAnimInstance::AnimNotify_NOT_NextAttack()
{
	OnNextAttackCheck.Broadcast();
}

void UPCWAnimInstance::AnimNotify_NOT_EndAttack()
{
	OnEndAttack.Broadcast();
}

void UPCWAnimInstance::AnimNotify_NOT_StartHit()
{
	OnStartHit.Broadcast();
}

void UPCWAnimInstance::AnimNotify_NOT_EndHit()
{
	OnEndHit.Broadcast();
}

FName UPCWAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	return FName(*FString::Printf(TEXT("Light%d"), Section));
}

MontageType UPCWAnimInstance::CheckMontage(UAnimMontage* Montage)
{
	if (Montage == AttackMontage || Montage == AttackMontage1 || Montage == AttackMontage2 || Montage == AttackMontage3 || Montage == AttackMontage4)
		return MontageType::Attack;
	else if (Montage == DeathMontage)
		return MontageType::Death;
	else if (Montage == DodgeMontage)
		return MontageType::Dodge;
	else
		return MontageType::None;
}