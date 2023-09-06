// Fill out your copyright notice in the Description page of Project Settings.


#include "PBEHVAnimInstance.h"
#include "PBEHViking.h"

UPBEHVAnimInstance::UPBEHVAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/Enemy/E_Human/EH_Viking/EHV_Animation/EHVA_BP/EHVA_AttackMontage.EHVA_AttackMontage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HIT_MONTAGE(TEXT("/Game/Enemy/E_Human/EH_Viking/EHV_Animation/EHVA_BP/EHVA_HitMontage.EHVA_HitMontage"));
	if (HIT_MONTAGE.Succeeded())
	{
		HitMontage = HIT_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DEATH_MONTAGE(TEXT("/Game/Enemy/E_Human/EH_Viking/EHV_Animation/EHVA_BP/EHVA_DeathMontage.EHVA_DeathMontage"));
	if (DEATH_MONTAGE.Succeeded())
	{
		DeathMontage = DEATH_MONTAGE.Object;
	}
}

void UPBEHVAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Character = Cast<APBEHViking>(TryGetPawnOwner());
}

void UPBEHVAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UPBEHVAnimInstance::PlayMontage(MontageType Type)
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

MontageType UPBEHVAnimInstance::CheckMontage(UAnimMontage* Montage)
{
	if (Montage == AttackMontage)
		return MontageType::Attack;
	else if (Montage == DeathMontage)
		return MontageType::Death;
	else if (Montage == HitMontage)
		return MontageType::Hit;
	else
		return MontageType::None;
}