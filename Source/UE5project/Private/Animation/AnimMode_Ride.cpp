// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimMode_Ride.h"
#include "Characters/Player/CharacterBaseAnimInstance.h"

void UAnimMode_Ride::Tick(float DeltaSeconds)
{
	if (!Character.IsValid() || !AnimInst.IsValid()) return;

	auto* Ch = Character.Get();
	auto* Anim = AnimInst.Get();

	Anim->CurRideStance = Ch->GetCurRideStance();

	if (Anim->CurRideStance == ERideStance::Riding)
	{
		Anim->Speed = Ch->GetRideSpeed();
		Anim->Direction = Ch->GetRideDirection();
	}
}
