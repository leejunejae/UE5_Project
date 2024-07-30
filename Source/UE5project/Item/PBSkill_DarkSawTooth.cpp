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
	SkillMovement->MaxSpeed = 3000.0f;
	SkillMovement->bRotationFollowsVelocity = true;
	SkillMovement->bAutoActivate = true;
	Tags.Add("Projectile");
}

void APBSkill_DarkSawTooth::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(ReturnTimerHandle, this, &APBSkill_DarkSawTooth::ReturnTimer, 3.0f, false);
}

void APBSkill_DarkSawTooth::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!(OtherActor->ActorHasTag("Player")))
		return;
	Destroy();
}

void APBSkill_DarkSawTooth::ReturnTimer()
{
	FVector StartDir = GetActorLocation();
	FVector DestDir = Target->GetActorLocation();
	FVector Direction = (DestDir - StartDir).GetSafeNormal();
	SkillMovement->Velocity = Direction * 3000.0f;
}