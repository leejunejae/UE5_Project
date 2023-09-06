// Fill out your copyright notice in the Description page of Project Settings.


#include "PBEHAnimInstance.h"
#include "PBEHuman.h"

UPBEHAnimInstance::UPBEHAnimInstance()
{
	CurrentSpeed = 0.0f;
	
}

void UPBEHAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

}

void UPBEHAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (Character)
	{
		Speed = Character->GetVelocity().Size();
		Direction = CalculateDirection(Character->GetVelocity(), Character->GetActorRotation());
		IsDead = Character->GetHealth_Implementation() < 0.0f;
	}
}