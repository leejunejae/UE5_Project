// Fill out your copyright notice in the Description page of Project Settings.


#include "PBEHAnimInstance.h"
#include "PBEHuman.h"

UPBEHAnimInstance::UPBEHAnimInstance()
{
	CurrentSpeed = 0.0f;
	IsInAir = false;
}

void UPBEHAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Character = Cast<APBEHuman>(TryGetPawnOwner());
}