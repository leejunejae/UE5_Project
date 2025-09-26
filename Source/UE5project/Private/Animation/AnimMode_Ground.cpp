// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimMode_Ground.h"

#include "Characters/Player/CharacterBase.h"
#include "Characters/Player/CharacterBaseAnimInstance.h"
#include "Characters/Components/BaseCharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UAnimMode_Ground::Tick(float DeltaSeconds)
{
	if (!Character.IsValid() || !AnimInst.IsValid()) return;

	auto* Ch = Character.Get();
	auto* Anim = AnimInst.Get();

	//Anim->CurGroundStance = Ch->CurGroundStance();

	Anim->Speed = Ch->GetVelocity().Length();
	Anim->Direction = Anim->GetAnimDirection(DeltaSeconds);
	Anim->IsInAir = Ch->GetMovementComponent()->IsFalling();
	Anim->IsJumping = Anim->IsFalling = Anim->IsLanding = false;

	if (Anim->IsInAir)
	{
		Ch->GetVelocity().Z > 0.0f ? Anim->IsJumping = true : Anim->IsFalling = true;
		if (auto* BaseMovement = Cast<UBaseCharacterMovementComponent>(Ch->GetMovementComponent()))
		{
			float GroundDistance = BaseMovement->GetGroundDistance(false);
			Anim->IsLanding = GroundDistance < 50.0f;
		}
	}

	Anim->MovementAlpha = FMath::GetRangePct(400.0f, 600.0f, Anim->Speed);

	// Quick Turn
	if (Anim->GetCurveValue(FName("TurnLock")) > 0.0f || Anim->Speed < 200.0f)
		Anim->bQuickTurn = false;
	else
	{
		FVector MovementDirection = Ch->GetVelocity().GetSafeNormal();
		FVector AccelDirection = Ch->GetLastMovementInputVector().GetSafeNormal();
		float AngleDotProduct = FVector::DotProduct(MovementDirection, AccelDirection);
		Anim->bQuickTurn = AngleDotProduct < -0.5f;
	}

	// Foot IK
	TTuple<FVector, float> TraceLeftFoot = Anim->FootTrace("Foot_L");
	TTuple<FVector, float> TraceRightFoot = Anim->FootTrace("Foot_R");

	Anim->FootRotation(DeltaSeconds, TraceLeftFoot.Key, &Anim->LeftFootRotator, 20.0f);
	Anim->FootRotation(DeltaSeconds, TraceRightFoot.Key, &Anim->RightFootRotator, 20.0f);

	Anim->PelvisOffset = FMath::FInterpTo(Anim->PelvisOffset,
		UKismetMathLibrary::Min(TraceLeftFoot.Value, TraceRightFoot.Value), DeltaSeconds, 5.0f);

	Anim->LeftFootGroundOffset.Z = FMath::FInterpTo(Anim->LeftFootGroundOffset.Z,
		TraceLeftFoot.Value - Anim->PelvisOffset, DeltaSeconds, 5.0f);
	Anim->RightFootGroundOffset.Z = FMath::FInterpTo(Anim->RightFootGroundOffset.Z,
		TraceRightFoot.Value - Anim->PelvisOffset, DeltaSeconds, 5.0f);
}