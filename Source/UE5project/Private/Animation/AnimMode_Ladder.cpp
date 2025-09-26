// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimMode_Ladder.h"

#include "Characters/Player/CharacterBase.h"
#include "Characters/Player/CharacterBaseAnimInstance.h"
#include "Interaction/Climb/Components/ClimbComponent.h"

void UAnimMode_Ladder::Tick(float DeltaSeconds)
{
	if (!Character.IsValid() || !AnimInst.IsValid()) return;

	auto* Ch = Character.Get();
	auto* Anim = AnimInst.Get();

	Anim->CurLadderStance = Ch->GetClimbComponent()->GetLadderStance_Native();

	Anim->LeftHandLadderOffset = Ch->GetClimbComponent()->GetLimbIKTarget(ELimbList::HandL);
	Anim->RightHandLadderOffset = Ch->GetClimbComponent()->GetLimbIKTarget(ELimbList::HandR);
	Anim->LeftFootLadderOffset = Ch->GetClimbComponent()->GetLimbIKTarget(ELimbList::FootL);
	Anim->RightFootLadderOffset = Ch->GetClimbComponent()->GetLimbIKTarget(ELimbList::FootR);
}
