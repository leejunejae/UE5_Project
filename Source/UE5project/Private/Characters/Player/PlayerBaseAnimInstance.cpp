// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/PlayerBaseAnimInstance.h"
#include "Characters/Player/PlayerBase.h"
#include "Animation/AnimMode_Ride.h"

void UPlayerBaseAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Character = Cast<APlayerBase>(Character); // ★ Player 전용

	if (Character)
	{
		AnimModeMap.Add(ECharacterState::Ride, NewObject<UAnimMode_Ride>(this));
	}
}

void UPlayerBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
}

void UPlayerBaseAnimInstance::AnimNotify_NOT_MountEnd()
{
	OnMountEnd.Broadcast();
}

void UPlayerBaseAnimInstance::AnimNotify_NOT_DisMountEnd()
{
	OnDisMountEnd.Broadcast();
}