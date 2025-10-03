// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Player/CharacterBaseAnimInstance.h"
#include "PlayerBaseAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API UPlayerBaseAnimInstance : public UCharacterBaseAnimInstance
{
	GENERATED_BODY()
	
	friend class UAnimMode_Ground_Player;
	friend class UAnimMode_Ride;

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

#pragma region Locomotion
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stance, Meta = (AllowPrivateAccess = true))
		ELocomotionState CurGroundStance;
#pragma endregion Locomotion

#pragma region Ride
public:
	FOnAnimInstanceMulDel OnMountEnd;
	FOnAnimInstanceMulDel OnDisMountEnd;

private:
	UFUNCTION()
		void AnimNotify_NOT_MountEnd();

	UFUNCTION()
		void AnimNotify_NOT_DisMountEnd();
#pragma endregion 
};
