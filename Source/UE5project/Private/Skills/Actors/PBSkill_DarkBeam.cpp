// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/Actors/PBSkill_DarkBeam.h"

APBSkill_DarkBeam::APBSkill_DarkBeam()
{
	SkillMovement->bRotationFollowsVelocity = true;
	SkillMovement->bAutoActivate = false;

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>H_DARKBEAM(TEXT("/Script/Niagara.NiagaraSystem'/Game/Enemy/E_Human/EH_Heretic/EHH_MagicParticle/NS_IonLaser.NS_IonLaser'"));
	if (H_DARKBEAM.Succeeded())
	{
		SkillNiagara->SetAsset(H_DARKBEAM.Object);
	}

	SkillNiagara->SetFloatParameter("_Length", 2.0f);

	SkillNiagara->bAutoActivate = true;
}

void APBSkill_DarkBeam::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(ActiveTimerHandle, this, &APBSkill_DarkBeam::ActiveTimer, 2.0f, false);
}

void APBSkill_DarkBeam::ActiveTimer()
{
	Destroy();
}
