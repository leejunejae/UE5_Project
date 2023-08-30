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

void UPBEHVAnimInstance::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.0f);
}