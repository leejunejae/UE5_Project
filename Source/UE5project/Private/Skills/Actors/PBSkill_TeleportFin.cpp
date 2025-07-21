// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/Actors/PBSkill_TeleportFin.h"

APBSkill_TeleportFin::APBSkill_TeleportFin()
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PARTICLE(TEXT("/Game/Enemy/E_Human/EH_Heretic/EHH_MagicParticle/P_Portal_Teleport_Exit.P_Portal_Teleport_Exit"));
	if (PARTICLE.Succeeded())
	{
		SkillParticle->SetTemplate(PARTICLE.Object);
	}
	SkillParticle->SetRelativeScale3D(FVector(2.0f, 2.0f, 2.0f));
	SkillParticle->CustomTimeDilation = 0.1f;
}