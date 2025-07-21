// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/Actors/PBSkill_DarkSpear.h"

APBSkill_DarkSpear::APBSkill_DarkSpear()
{
	SkillMovement->bRotationFollowsVelocity = true;
	SkillMovement->bAutoActivate = false;

	SpearDOTNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SpearDOTNiagara"));
	SpearDOTNiagara->SetupAttachment(SkillCol);
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>H_DARKSPEAR(TEXT("/Game/Enemy/E_Human/EH_Heretic/EHH_MagicParticle/NS_DarkSpear.NS_DarkSpear"));
	if (H_DARKSPEAR.Succeeded())
	{
		SkillNiagara->SetAsset(H_DARKSPEAR.Object);
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>H_DOT(TEXT("/Game/Enemy/E_Human/EH_Heretic/EHH_MagicParticle/NS_Hit_Constant_IonLaser.NS_Hit_Constant_IonLaser"));
	if (H_DOT.Succeeded())
	{
		SpearDOTNiagara->SetAsset(H_DOT.Object);
	}

	SkillNiagara->bAutoActivate = true;
	SpearDOTNiagara->bAutoActivate = false;

	SkillMovement->InitialSpeed = 0.0f;
	SkillMovement->MaxSpeed = 3000.0f;
	SkillMovement->bRotationFollowsVelocity = true;
	SkillMovement->bAutoActivate = true;
	Speed = 1000.0f;
	Tags.Add("Projectile");
}

void APBSkill_DarkSpear::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->ActorHasTag("Player"))
		Destroy();
	else
	{
		SpearDOTNiagara->Activate();
		SkillNiagara->Deactivate();
	}
}
