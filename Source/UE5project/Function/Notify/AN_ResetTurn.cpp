// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_ResetTurn.h"
#include "../IAnimInstance.h"

void UAN_ResetTurn::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp->GetAnimInstance()->GetClass()->ImplementsInterface(UIAnimInstance::StaticClass()))
	{
		IIAnimInstance::Execute_ResetTurn(MeshComp->GetAnimInstance());
	}
}