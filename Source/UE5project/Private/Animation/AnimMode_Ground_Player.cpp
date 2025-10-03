// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimMode_Ground_Player.h"
#include "Characters/Player/PlayerBase.h"
#include "Characters/Player/PlayerBaseAnimInstance.h"

void UAnimMode_Ground_Player::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!Character.IsValid() || !AnimInst.IsValid()) return;
	auto* Ch = Cast<APlayerBase>(Character.Get());
	auto* Anim = Cast<UPlayerBaseAnimInstance>(AnimInst.Get());

	Anim->CurGroundStance = Ch->GetCurGroundStance();

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