// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Mode/AnimMode_Ground_Player.h"
#include "Characters/Player/PlayerBase.h"
#include "Characters/Player/PlayerBaseAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

void UAnimMode_Ground_Player::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!Character.IsValid() || !AnimInst.IsValid()) return;
	auto* Ch = Cast<APlayerBase>(Character.Get());
	auto* Anim = Cast<UPlayerBaseAnimInstance>(AnimInst.Get());

	// 가속 확인
	FVector WorldAcceleration = Character->GetCharacterMovement()->GetCurrentAcceleration() * FVector(1.0f, 1.0f, 0.0f);
	FVector LocalAcceleration = Character->GetActorRotation().UnrotateVector(WorldAcceleration);
	float AccelerationLength = LocalAcceleration.SizeSquared();
	AnimInst->IsAccelerating = !FMath::IsNearlyZero(AccelerationLength);

	if (Anim->Speed > 100.0f)
	{
		Anim->LeftFootGroundOffset.X = FMath::FInterpTo(Anim->LeftFootGroundOffset.X, 5.0f, DeltaSeconds, 5.0f);
		Anim->RightFootGroundOffset.X = FMath::FInterpTo(Anim->RightFootGroundOffset.X, -5.0f, DeltaSeconds, 5.0f);
	}
	else
	{
		Anim->LeftFootGroundOffset.X = FMath::FInterpTo(Anim->LeftFootGroundOffset.X, 0.0f, DeltaSeconds, 5.0f);
		Anim->RightFootGroundOffset.X = FMath::FInterpTo(Anim->RightFootGroundOffset.X, 0.0f, DeltaSeconds, 5.0f);
	}
}