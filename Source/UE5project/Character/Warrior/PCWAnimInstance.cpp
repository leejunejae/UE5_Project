// Fill out your copyright notice in the Description page of Project Settings.


#include "PCWAnimInstance.h"
#include "Warrior.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"

UPCWAnimInstance::UPCWAnimInstance()
{
	IsInAir = false;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DEATH_MONTAGE(TEXT("/Game/Character/C_Warrior/CW_Animation/CWA_BP/CWAB_DeathMontage.CWAB_DeathMontage"));
	if (DEATH_MONTAGE.Succeeded())
	{
		DeathMontage = DEATH_MONTAGE.Object;
	}
}

void UPCWAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Character = Cast<AWarrior>(TryGetPawnOwner());
	SeedSwitch = true;
}

void UPCWAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (Character)
	{
		CurStance = Character->GetStance();
		IsMovementInput = Character->GetIsMovementInput();
		IsInAir = Character->GetMovementComponent()->IsFalling();
		IsAttack = Character->IsAttacking();
		IsAttackInput = Character->IsAttackInput();
		IsBlock = Character->IsBlocking();
		IsParry = Character->IsParrying();
		IsRoll = Character->IsRolling();
		Response = Character->GetCharResponse();
		IsRide = Character->IsRiding();
		IsEscape = Character->GetVertical() > 1.0f ? true : false;
		IsRightHand = Character->GetCurHand();
		ComboCount = Character->CheckCombo();
		IsAccelerating = Character->GetCharacterMovement()->GetCurrentAcceleration().Length() > 0.0f ? true : false;

		if (!IsRide)
		{
			//Speed = Character->GetVelocity().Size();

			float SpeedX = FVector::DotProduct(Character->GetVelocity(), Character->GetActorForwardVector());
			float SpeedY = FVector::DotProduct(Character->GetVelocity(), Character->GetActorRightVector());
			float VelocityLength = Character->GetVelocity().Length();
			Speed = FMath::FInterpTo(Speed, VelocityLength, DeltaSeconds, 20.0f);
			Direction = CalculateDirection(Character->GetVelocity(), Character->GetActorRotation());
			if (IsAccelerating)
			{
				if (Speed > 200.0f)
					LastDirection = Direction;
			}
			else
			{
				
			}
			InputDirectionX = Character->GetInputDirection().X;
			InputDirectionY = Character->GetInputDirection().Y;

			if (InputDirectionX == 0.0f && InputDirectionY == 0.0f)
				InputDirection = 180.0f;
			else
				InputDirection = UKismetMathLibrary::DegAtan2(InputDirectionX, InputDirectionY);

			float BlendAlpha = IsBlock ? 0.99f : 0.01f;
			BlockBlend = FMath::FInterpTo(BlockBlend, BlendAlpha, DeltaSeconds, 10.0f);

			SetPitch();
			SetRootYawOffset();

			float LeftFootOffsetLast;
			float RightFootOffsetLast;
			float PelvisOffsetLast;

			LeftFootOffsetLast = LeftFootOffset;
			RightFootOffsetLast = RightFootOffset;
			PelvisOffsetLast = PelvisOffset;

			TTuple<FVector, float> TraceLeftFoot = FootTrace("Foot_L");
			TTuple<FVector, float> TraceRightFoot = FootTrace("Foot_R");

			UE_LOG(LogTemp, Warning, TEXT("LeftFootOffset = %f"), TraceLeftFoot.Value);
			UE_LOG(LogTemp, Warning, TEXT("RightFootOffset = %f"), TraceRightFoot.Value);

			FootRotation(DeltaSeconds, TraceLeftFoot.Key, &LeftFootRotator, 20.0f);
			FootRotation(DeltaSeconds, TraceRightFoot.Key, &RightFootRotator, 20.0f);
			//UE_LOG(LogTemp, Warning, TEXT("LeftFootRotator (Pitch : %f, Yaw : %f, Roll : %f) "), LeftFootRotator.Pitch, LeftFootRotator.Yaw, LeftFootRotator.Roll);
			//UE_LOG(LogTemp, Warning, TEXT("RightFootRotator (Pitch : %f, Yaw : %f, Roll : %f)"), RightFootRotator.Pitch, RightFootRotator.Yaw, RightFootRotator.Roll);
			
			//PelvisOffset = UKismetMathLibrary::Min(TraceLeftFoot.Value, TraceRightFoot.Value);
			PelvisOffset = FMath::FInterpTo(PelvisOffsetLast, UKismetMathLibrary::Min(TraceLeftFoot.Value, TraceRightFoot.Value), DeltaSeconds, 15.0f);
			LeftFootOffset = FMath::FInterpTo(LeftFootOffsetLast, TraceLeftFoot.Value - PelvisOffset, DeltaSeconds, 15.0f);
			RightFootOffset = FMath::FInterpTo(RightFootOffsetLast, -1.0f * (TraceRightFoot.Value - PelvisOffset), DeltaSeconds, 15.0f);

			/*
			if (Speed > 100.0f)
			{
				PelvisOffset = FMath::FInterpTo(PelvisOffsetLast, UKismetMathLibrary::Min(TraceLeftFoot.Value, TraceRightFoot.Value), DeltaSeconds, 1.0f);
				LeftFootOffset = FMath::FInterpTo(LeftFootOffsetLast, TraceLeftFoot.Value - PelvisOffset, DeltaSeconds, 1.0f);
				RightFootOffset = FMath::FInterpTo(RightFootOffsetLast, -1.0f * (TraceRightFoot.Value - PelvisOffset), DeltaSeconds, 1.0f);
			}
			else
			{
				PelvisOffset = FMath::FInterpTo(PelvisOffsetLast, UKismetMathLibrary::Min(TraceLeftFoot.Value, TraceRightFoot.Value), DeltaSeconds, 15.0f);
				LeftFootOffset = FMath::FInterpTo(LeftFootOffsetLast, TraceLeftFoot.Value - PelvisOffset, DeltaSeconds, 15.0f);
				RightFootOffset = FMath::FInterpTo(RightFootOffsetLast, -1.0f * (TraceRightFoot.Value - PelvisOffset), DeltaSeconds, 15.0f);
			}
			*/
		}
		else
		{
			MountRight = Character->GetMountDir();
			Speed = Character->GetVertical();
			Direction = Character->GetHorizontal();
		}
	}
}

TTuple<FVector, float> UPCWAnimInstance::FootTrace(FName SocketName)
{
	FVector FootLoc = Character->GetMesh()->GetSocketLocation(SocketName);
	float TraceDistance = 50.0f;
	FVector Start = FVector(FootLoc.X, FootLoc.Y, Character->GetActorLocation().Z);
	FVector End = Start - FVector(0.0f, 0.0f, Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + TraceDistance);

	FHitResult DistHitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.bTraceComplex = true;
	enum ECollisionChannel TraceChannel = Speed > 100.0f ? ECC_Visibility : ECC_GameTraceChannel8;

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		DistHitResult,
		Start,
		End,
		TraceChannel,
		CollisionParams
	);

	//FColor DrawColor = bHit ? FColor::Green : FColor::Red;
	//DrawDebugLine(GetWorld(), Start, End, DrawColor, false, 1.0f);

	if (bHit)
	{
		float Offset = ((DistHitResult.ImpactPoint - DistHitResult.TraceEnd).Length() + 5.0f) - TraceDistance;
		return MakeTuple(DistHitResult.ImpactNormal, Offset);
	}
	else
	{
		return MakeTuple(FVector::ZeroVector, 0.0f);
	}
}

void UPCWAnimInstance::FootRotation(float DeltaTime, FVector TargetNormal, FRotator *FootRotator, float fInterpSpeed)
{
	float AtanX = -1.0f * UKismetMathLibrary::DegAtan2(TargetNormal.X, TargetNormal.Z);
	float AtanY = UKismetMathLibrary::DegAtan2(TargetNormal.Y, TargetNormal.Z);
	FRotator TargetRotator = FRotator(AtanX, 0.0f, AtanY);

	*FootRotator = UKismetMathLibrary::RInterpTo(*FootRotator, TargetRotator, DeltaTime, fInterpSpeed);
}

void UPCWAnimInstance::FootPlacement(float DeltaTime, float TargetValue, float* FootValue, float InterpSpeed)
{
	*FootValue = UKismetMathLibrary::FInterpTo(*FootValue, TargetValue, DeltaTime, InterpSpeed);
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
	case MontageType::Death:
		Montage_Play(DeathMontage, 1.0f);
		break;
	default:
		break;
	}
}

void UPCWAnimInstance::AnimNotify_NOT_ParryEnd()
{
	OnParryEnd.Broadcast();
}

void UPCWAnimInstance::AnimNotify_NOT_NextAttack()
{
	OnNextAttackCheck.Broadcast();
}

void UPCWAnimInstance::AnimNotify_NOT_SetAttackDirection()
{
	OnSetAttackDirection.Broadcast();
}

void UPCWAnimInstance::AnimNotify_NOT_EndAttack()
{
	OnEndAttack.Broadcast();
}

void UPCWAnimInstance::AnimNotify_NOT_MountEnd()
{
	OnMountEnd.Broadcast();
}

void UPCWAnimInstance::AnimNotify_NOT_DisMountEnd()
{
	OnDisMountEnd.Broadcast();
}

void UPCWAnimInstance::AnimNotify_NOT_ResetHurt()
{
	//UE_LOG(LogTemp, Warning, TEXT("ResetHurt"));
	OnResetHurt.Broadcast();
}

void UPCWAnimInstance::AnimNotify_NOT_EnterLocomotion()
{
	OnEnterLocomotion.Broadcast();
}

void UPCWAnimInstance::AnimNotify_NOT_LeftLocomotion()
{
	//UE_LOG(LogTemp, Warning, TEXT("ResetHurt"));
	OnLeftLocomotion.Broadcast();
}

void UPCWAnimInstance::AnimNotify_NOT_DodgeEnd()
{
	OnDodgeEnd.Broadcast();
}

void UPCWAnimInstance::AnimNotify_NOT_DodgeStart()
{
	OnDodgeStart.Broadcast();
}

void UPCWAnimInstance::AnimNotify_NOT_AttackStart()
{
	OnAttackStart.Broadcast();
}

void UPCWAnimInstance::AnimNotify_NOT_CanDodge()
{
	OnCanDodge.Broadcast();
}

/*
void UPCWAnimInstance::AnimNotify_NOT_EquipEnd()
{
	OnEquipEnd.Broadcast();
}

void UPCWAnimInstance::AnimNotify_NOT_HolsterEnd()
{
	OnHolsterEnd.Broadcast();
}
*/

FName UPCWAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	return FName(*FString::Printf(TEXT("Light%d"), Section));
}

MontageType UPCWAnimInstance::CheckMontage(UAnimMontage* Montage)
{
	if (Montage == DeathMontage)
		return MontageType::Death;
	else
		return MontageType::None;
}