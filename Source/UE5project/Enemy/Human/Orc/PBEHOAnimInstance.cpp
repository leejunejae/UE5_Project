// Fill out your copyright notice in the Description page of Project Settings.


#include "PBEHOAnimInstance.h"
#include "PBEHOrc.h"

UPBEHOAnimInstance::UPBEHOAnimInstance()
{
	/*
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
	*/
}

void UPBEHOAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	//Character = Cast<APBEHViking>(TryGetPawnOwner());
}

void UPBEHOAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}