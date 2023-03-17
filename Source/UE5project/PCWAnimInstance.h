// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PCWAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API UPCWAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPCWAnimInstance();
	class APCWarrior* Character = nullptr;
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
	float Speed = 0.0f;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
	float Direction = 0.0f;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
	float YawActual = 0.0f;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
	float Pitch = 0.0f;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
	float Yaw = 0.0f;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
	float RootYawOffset = 0.0f;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
	float DistanceCurveValue = 0.0f;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
	float MaxTurnAngle = 90.0f;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
	FName DistanceToPivot = TEXT("DistanceToPivot");
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
	FName Turning = TEXT("Turning");

	void SetPitch();
	void SetRootYawOffset();
	void SetLeftHandPosition();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
	float CurrentSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
	int32 MoveFlag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
	bool IsInAir;

	float YawLastTick = 0.0f;
	float YawChangeOverFrame = 0.0f;
	float DistanceCurveValueLastFrame = 0.0f;
	float TurnDirection = 0.0f;
	float ABSRootYawOffset = 0.0f;
	float DistanceCurveDifference = 0.0f;
	float YawToSubtract = 0.0f;
	float YawMultiplier = 0.0f;
};