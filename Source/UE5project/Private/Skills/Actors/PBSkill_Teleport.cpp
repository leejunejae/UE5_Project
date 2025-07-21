// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/Actors/PBSkill_Teleport.h"

APBSkill_Teleport::APBSkill_Teleport()
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PARTICLE(TEXT("/Game/Enemy/E_Human/EH_Heretic/EHH_MagicParticle/P_Portal_Teleport_Enter.P_Portal_Teleport_Enter"));
	if (PARTICLE.Succeeded())
	{
		SkillParticle->SetTemplate(PARTICLE.Object);
	}
	SkillParticle->SetRelativeScale3D(FVector(2.0f, 2.0f, 2.0f));
	SkillParticle->CustomTimeDilation = 0.1f;
}