// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/Human/HereticKnight/PBEHHSAnimInstance.h"
#include "Characters/Enemies/Human/HereticKnight/PBEHHereticSoldier.h"
#include "Kismet/KismetMathLibrary.h"


UPBEHHSAnimInstance::UPBEHHSAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> APPEAR_MONTAGE(TEXT("/Game/Enemy/E_Human/EH_HereticSoldier/EHHS_Animation/EHHS_BP/EHHS_Appear.EHHS_Appear"));
	if (APPEAR_MONTAGE.Succeeded())
	{
		AppearMontage = APPEAR_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> Combo1_MONTAGE(TEXT("/Game/Enemy/E_Human/EH_HereticSoldier/EHHS_Animation/EHHS_BP/Combo1.Combo1"));
	if (Combo1_MONTAGE.Succeeded())
	{
		Combo1 = Combo1_MONTAGE.Object;
	}
}

void UPBEHHSAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	HSoldier = Cast<APBEHHereticSoldier>(TryGetPawnOwner());
}

void UPBEHHSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (HSoldier)
	{
		CurSkill = HSoldier->CheckSkill();
		IsAttack = HSoldier->CheckBool(HSoldierVar::IsAttack);
		IsHit = HSoldier->CheckBool(HSoldierVar::IsHit);
		IsHitLarge = HSoldier->CheckBool(HSoldierVar::IsHitLarge);
		IsBlock = HSoldier->CheckBool(HSoldierVar::IsBlock);
		IsDefenseMode = HSoldier->CheckBool(HSoldierVar::IsDefenseMode);
		IsBlockBreak = HSoldier->CheckBool(HSoldierVar::IsBlockBreak);
		IsParried = HSoldier->CheckBool(HSoldierVar::IsParried);
		IsDash = HSoldier->IsDashing();
		IsLookAt = HSoldier->IsLookingAt();
		LookAtTarget = HSoldier->GetLookingTarget();
		ComboCount = HSoldier->CheckCombo();

		if (IsLookAt && LookAtTarget != nullptr)
		{
			LookAtLocation = LookAtTarget->GetActorLocation();
			AngleFromTarget = UKismetMathLibrary::NormalizedDeltaRotator(HSoldier->GetActorRotation(), UKismetMathLibrary::FindLookAtRotation(HSoldier->GetActorLocation(), LookAtLocation));
			float AngleFromTargetYaw = AngleFromTarget.Yaw;
			FMath::Abs(AngleFromTargetYaw) < 30.0f ? AimAlpha = 1.0f : AimAlpha = 1.0f - (((FMath::Abs(AngleFromTargetYaw) - 30.0f) / 150.0f));
			if (Speed == 0.0f && FMath::Abs(AngleFromTargetYaw) > 60.0f && !IsTurning)
			{
				UpdateAngle = AngleFromTargetYaw;
				IsTurning = true;
			}

			

			//UE_LOG(LogTemp, Warning, TEXT("TargetLocation = X : %f, Y : %f, Z : %f "), LookAtLocation.X, LookAtLocation.Y, LookAtLocation.Z);
			//UE_LOG(LogTemp, Warning, TEXT("ActorRotation : %f "), HSoldier->GetActorRotation().Yaw);
			//UE_LOG(LogTemp, Warning, TEXT("FindLookAtRotation : %f "), UKismetMathLibrary::FindLookAtRotation(HSoldier->GetActorLocation(), LookAtLocation).Yaw);
			//UE_LOG(LogTemp, Warning, TEXT("AngleFromTarget : %f "), AngleFromTarget.Yaw);
			//UE_LOG(LogTemp, Warning, TEXT("YawChangeOverFrame : %f \n"), YawChangeOverFrame);
			/*
			CanLookAt = AngleFromTargetYaw > -60.0f && AngleFromTargetYaw < 30.0f ? true : false;
			if (!CanLookAt)
			{
				LookAtLocation = LastFrameLookAtLocation;
			}
			else
			{
				LastFrameLookAtLocation = LookAtLocation;
			}
			*/

			
			//CanLookAt = FMath::Abs(DeltaRot.Yaw) < 70.0f ? true : false;
		}

		InputDirectionX = HSoldier->GetDashDirection().X;
		InputDirectionY = HSoldier->GetDashDirection().Y;

		

		if (InputDirectionX == 0.0f && InputDirectionY == 0.0f)
			InputDirection = 180.0f;
		else
			InputDirection = UKismetMathLibrary::DegAtan2(InputDirectionX, InputDirectionY);

		//UE_LOG(LogTemp, Warning, TEXT("%f "), InputDirection);
		//SetPitch();
		//SetRootYawOffset();
	}
}

void UPBEHHSAnimInstance::PlayMontage(FName MontageName)
{
	Super::PlayMontage(MontageName);

	if (MontageName == "Combo1")
	{
		Montage_Play(Combo1);
	}

}

MontageType UPBEHHSAnimInstance::CheckMontage(UAnimMontage* Montage)
{
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *Montage->GetName());
	if (Montage == AppearMontage)
		return MontageType::Appear;
	else if (Montage == AttackMontage)
		return MontageType::Attack;
	else
		return MontageType::None;
}

void UPBEHHSAnimInstance::AnimNotify_NOT_CanNextCombo()
{
	OnNextCombo.Broadcast();
}

void UPBEHHSAnimInstance::AnimNotify_NOT_EndCombo()
{
	OnEndCombo.Broadcast();
}

void UPBEHHSAnimInstance::AnimNotify_NOT_StartAttack()
{
	//UE_LOG(LogTemp, Warning, TEXT("StartAttack"));
	OnStartAttack.Broadcast();
}

void UPBEHHSAnimInstance::AnimNotify_NOT_EndAttack()
{
	//UE_LOG(LogTemp, Warning, TEXT("EndAttack"));
	OnEndAttack.Broadcast();
}

void UPBEHHSAnimInstance::AnimNotify_NOT_HitEnd()
{
	OnHitEnd.Broadcast();
}

void UPBEHHSAnimInstance::AnimNotify_NOT_HitLargeEnd()
{
	OnHitLargeEnd.Broadcast();
}

void UPBEHHSAnimInstance::AnimNotify_NOT_BlockBreakEnd()
{
	OnBlockBreakEnd.Broadcast();
}

void UPBEHHSAnimInstance::AnimNotify_NOT_DashEnd()
{
	OnDashEnd.Broadcast();
}

void UPBEHHSAnimInstance::AnimNotify_NOT_TurnEnd()
{
	IsTurning = false;
}

void UPBEHHSAnimInstance::AnimNotify_NOT_EnterLocomotion()
{
	OnEnterLocomotion.Broadcast();
}

void UPBEHHSAnimInstance::AnimNotify_NOT_LeftLocomotion()
{
	OnLeftLocomotion.Broadcast();
}
