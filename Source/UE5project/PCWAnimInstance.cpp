// Fill out your copyright notice in the Description page of Project Settings.


#include "PCWAnimInstance.h"
#include "PCWarrior.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UPCWAnimInstance::UPCWAnimInstance()
{
	CurrentSpeed = 0.0f;
	MoveFlag = 0;
	IsInAir = false;
}

void UPCWAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Character = Cast<APCWarrior>(TryGetPawnOwner());
}

void UPCWAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	CurrentSpeed = Character->GetVelocity().Size();
	if (Character)
	{
		FVector NewVelocity = Character->GetVelocity();
		MoveFlag = Character->CheckMFlag();
		IsInAir = Character->GetMovementComponent()->IsFalling();

		Direction = CalculateDirection(NewVelocity, Character->GetActorRotation());

		SetPitch();
		SetRootYawOffset();
		SetLeftHandPosition();
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
	if (Speed > 0.0f || IsAnyMontagePlaying())
	{
		RootYawOffset = 0.0f;
		return;
	}

	YawLastTick = Yaw;
	Yaw = Character->GetActorRotation().Yaw;
	YawChangeOverFrame = YawLastTick - Yaw;

	RootYawOffset = UKismetMathLibrary::NormalizeAxis(YawChangeOverFrame + RootYawOffset);

	if (GetCurveValue(Turning) > 0.0f)
	{
		DistanceCurveValueLastFrame = DistanceCurveValue;
		DistanceCurveValue = GetCurveValue(DistanceToPivot);

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

void UPCWAnimInstance::SetLeftHandPosition()
{

}