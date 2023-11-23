// Fill out your copyright notice in the Description page of Project Settings.


#include "PBSkill_DarkSawTooth.h"

APBSkill_DarkSawTooth::APBSkill_DarkSawTooth()
{
	SkillMovement->bRotationFollowsVelocity = true;
	SkillMovement->bAutoActivate = false;

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>H_DARKTORNADO(TEXT("/Game/Enemy/E_Human/EH_Heretic/EHH_MagicParticle/NS_Tooth.NS_Tooth"));
	if (H_DARKTORNADO.Succeeded())
	{
		SkillNiagara->SetAsset(H_DARKTORNADO.Object);
	}
	
	SkillNiagara->bAutoActivate = true;

	SkillCol->SetBoxExtent(FVector(52.0f, 52.0f, 15.0f));

	SkillMovement->InitialSpeed = 0.0f;
	SkillMovement->MaxSpeed = 300.0f;
	SkillMovement->bRotationFollowsVelocity = true;
	SkillMovement->bAutoActivate = true;
}