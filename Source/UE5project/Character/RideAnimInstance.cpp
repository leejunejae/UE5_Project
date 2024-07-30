// Fill out your copyright notice in the Description page of Project Settings.


#include "RideAnimInstance.h"
#include "Ride.h"

URideAnimInstance::URideAnimInstance()
{

}

void URideAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Character = Cast<ARide>(TryGetPawnOwner());
}

void URideAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (Character)
	{
		Horizontal = Character->CheckVar(RideVar::Horizontal);
		Vertical = Character->CheckVar(RideVar::Vertical);
		IsBreak = Character->CheckBool(RideVar::IsBreak);
	}
}
