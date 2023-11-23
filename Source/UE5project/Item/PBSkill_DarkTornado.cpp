// Fill out your copyright notice in the Description page of Project Settings.


#include "PBSkill_DarkTornado.h"

APBSkill_DarkTornado::APBSkill_DarkTornado()
{
	SkillMovement->bRotationFollowsVelocity = true;
	SkillMovement->bAutoActivate = false;

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>H_DARKTORNADO(TEXT("/Game/Enemy/E_Human/EH_Heretic/EHH_MagicParticle/NS_Tornado.NS_Tornado"));
	if (H_DARKTORNADO.Succeeded())
	{
		SkillNiagara->SetAsset(H_DARKTORNADO.Object);
	}
	SkillNiagara->bAutoActivate = true;
}