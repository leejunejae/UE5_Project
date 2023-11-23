// Fill out your copyright notice in the Description page of Project Settings.


#include "PBEMAnimInstance.h"
#include "PBEHHeretic.h"

UPBEMAnimInstance::UPBEMAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> TELEPORT_MONTAGE(TEXT("/Game/Enemy/E_Human/EH_Heretic/EHH_Animation/EHHA_BP/EHHA_Teleport.EHHA_Teleport"));
	if (TELEPORT_MONTAGE.Succeeded())
	{
		TeleportMontage = TELEPORT_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> FIREPROJ_MONTAGE(TEXT("/Game/Enemy/E_Human/EH_Heretic/EHH_Animation/EHHA_BP/EHHA_Projectile.EHHA_Projectile"));
	if (FIREPROJ_MONTAGE.Succeeded())
	{
		FireProjMontage = FIREPROJ_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SUMMON_MONTAGE(TEXT("/Game/Enemy/E_Human/EH_Heretic/EHH_Animation/EHHA_BP/EHHA_Projectile.EHHA_Projectile"));
	if (SUMMON_MONTAGE.Succeeded())
	{
		SummonMontage = SUMMON_MONTAGE.Object;
	}
}

void UPBEMAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Character = Cast<APBEHHeretic>(TryGetPawnOwner());
}

void UPBEMAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Character)
	{
		Speed = Character->GetVelocity().Size();
		Direction = CalculateDirection(Character->GetVelocity(), Character->GetActorRotation());
		IsSummon = Character->CheckBool(HereticVar::Summon);
	}
}

void UPBEMAnimInstance::PlayMontage(HereticMontage Type)
{
	switch (Type)
	{
	case HereticMontage::Teleport:
		Montage_Play(TeleportMontage);
		break;
	case HereticMontage::Attack:
		Montage_Play(FireProjMontage);
		break;
	case HereticMontage::Summon:
		Montage_Play(SummonMontage);
		break;
	}
}

HereticMontage UPBEMAnimInstance::CheckMontage(UAnimMontage* Montage)
{
	if (Montage == TeleportMontage)
		return HereticMontage::Teleport;
	else if(Montage == FireProjMontage)
		return HereticMontage::Attack;
	else
		return HereticMontage::None;
}