// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/Projectiles/PBProjectile_IceArrow.h"

APBProjectile_IceArrow::APBProjectile_IceArrow()
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PARTICLE(TEXT("/Game/InfinityBladeEffects/Effects/FX_Monsters/FX_Monster_Elemental/ICE/P_Elemental_Ice_Proj.P_Elemental_Ice_Proj"));
	if (PARTICLE.Succeeded())
	{
		ProjectileParticle->SetTemplate(PARTICLE.Object);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> SPAWNPART(TEXT("/Game/InfinityBladeEffects/Effects/FX_Monsters/FX_Monster_Spider/ICE/P_Ice_Proj_charge_01.P_Ice_Proj_charge_01"));
	if (SPAWNPART.Succeeded())
	{
		SpawnParticle = SPAWNPART.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> DESTPART(TEXT("/Game/InfinityBladeEffects/Effects/FX_Monsters/FX_Monster_Elemental/ICE/P_Elemental_Proj_Impact_Ice.P_Elemental_Proj_Impact_Ice"));
	if (DESTPART.Succeeded())
	{
		DestroyParticle = DESTPART.Object;
	}

	/*
	static ConstructorHelpers::FObjectFinder<USoundCue> DESTSOUND(TEXT("/Game/StarterContent/Audio/Explosion_Cue.Explosion_Cue"));
	if (DESTPART.Succeeded())
	{
		DestroySound = DESTSOUND.Object;
	}
	*/

	ProjectileCol->SetBoxExtent(FVector(50.0f, 13.0f, 13.0f));
	//ProjectileCol->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
	ProjectileParticle->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	ProjectileParticle->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
}

void APBProjectile_IceArrow::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (DestroyParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestroyParticle, Hit.Location);
	}

	if (DestroySound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), DestroySound, Hit.Location);
	}
	Destroy();
}