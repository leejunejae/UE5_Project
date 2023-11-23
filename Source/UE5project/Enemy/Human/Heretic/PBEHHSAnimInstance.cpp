// Fill out your copyright notice in the Description page of Project Settings.


#include "PBEHHSAnimInstance.h"

UPBEHHSAnimInstance::UPBEHHSAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/Enemy/E_Human/EH_HereticSoldier/EHHS_Animation/EHHS_BP/EHHS_AttackMontage.EHHS_AttackMontage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> APPEAR_MONTAGE(TEXT("/Game/Enemy/E_Human/EH_HereticSoldier/EHHS_Animation/EHHS_BP/EHHS_Appear.EHHS_Appear"));
	if (APPEAR_MONTAGE.Succeeded())
	{
		AppearMontage = APPEAR_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SWOOP_MONTAGE(TEXT("/Game/Enemy/E_Human/EH_HereticSoldier/EHHS_Animation/EHHS_BP/EHHS_SwoopMontage.EHHS_SwoopMontage"));
	if (SWOOP_MONTAGE.Succeeded())
	{
		SwoopMontage = SWOOP_MONTAGE.Object;
	}
}

void UPBEHHSAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UPBEHHSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UPBEHHSAnimInstance::PlayMontage(MontageType Type)
{
	Super::PlayMontage(Type);
	switch (Type)
	{
	case MontageType::Swoop:
		Montage_Play(SwoopMontage, 1.0f);
		break;
	case MontageType::Appear:
		Montage_Play(AppearMontage, 1.0f);
		break;
	default:
		break;
	}
}

MontageType UPBEHHSAnimInstance::CheckMontage(UAnimMontage* Montage)
{
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *Montage->GetName());
	if (Montage == AttackMontage)
		return MontageType::Attack;
	else if (Montage == AppearMontage)
		return MontageType::Appear;
	else if (Montage == SwoopMontage)
		return MontageType::Swoop;
	else
		return MontageType::None;
}

void UPBEHHSAnimInstance::AnimNotify_NOT_Rush()
{
	OnRush.Broadcast();
}

void UPBEHHSAnimInstance::AnimNotify_NOT_Teleport()
{
	OnTeleport.Broadcast();
}