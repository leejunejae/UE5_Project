// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_SetNextGrip.h"
#include "../Interact/ClimbInterface.h"

void UAN_SetNextGrip::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (MeshComp)
	{
		if (MeshComp->GetOwner()->GetClass()->ImplementsInterface(UClimbInterface::StaticClass()))
		{
			IClimbInterface::Execute_SetNextGripDown(MeshComp->GetOwner(), TargetBone, Count);
		}
	}
}