// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/PBSkill.h"

// Sets default values
APBSkill::APBSkill()
{
	SkillParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SkillParticle"));
	SkillParticle->bAutoActivate = true;

	SkillCol = CreateDefaultSubobject<UBoxComponent>(TEXT("SkillCol"));
	SkillMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SkillMesh"));
	SkillNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SkillNiagara"));

	SetRootComponent(SkillCol);
	SkillMesh->SetupAttachment(SkillCol);
	SkillNiagara->SetupAttachment(SkillCol);

	SkillCol->SetEnableGravity(false);
	SkillMesh->SetEnableGravity(false);
}

