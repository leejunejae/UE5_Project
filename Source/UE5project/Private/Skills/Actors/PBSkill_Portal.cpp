// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/Actors/PBSkill_Portal.h"

APBSkill_Portal::APBSkill_Portal()
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PARTICLE(TEXT("/Game/Enemy/E_Human/EH_Heretic/EHH_MagicParticle/Proto_P_Gideon_Meteor_Portal.Proto_P_Gideon_Meteor_Portal"));
	if (PARTICLE.Succeeded())
	{
		SkillParticle->SetTemplate(PARTICLE.Object);
	}
	SkillParticle->SetRelativeScale3D(FVector(2.0f, 2.0f, 2.0f));
	SkillParticle->CustomTimeDilation = 0.1f;
}