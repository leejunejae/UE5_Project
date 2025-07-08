// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/Projectiles/PBProjectile_Fireball.h"

APBProjectile_Fireball::APBProjectile_Fireball()
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PARTICLE(TEXT("/Game/InfinityBladeEffects/Effects/FX_Mobile/Fire/combat/P_FireBall_Strong.P_FireBall_Strong"));
	if (PARTICLE.Succeeded())
	{
		ProjectileParticle->SetTemplate(PARTICLE.Object);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> DESTPART(TEXT("/Game/InfinityBladeEffects/Effects/FX_Mobile/Fire/combat/P_FireBall_Strong_Death_01.P_FireBall_Strong_Death_01"));
	if (DESTPART.Succeeded())
	{
		DestroyParticle = DESTPART.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> DESTSOUND(TEXT("/Game/StarterContent/Audio/Explosion_Cue.Explosion_Cue"));
	if (DESTPART.Succeeded())
	{
		DestroySound = DESTSOUND.Object;
	}

	ProjectileCol->SetBoxExtent(FVector(40.0f, 15.0f, 15.0f));
	ProjectileParticle->SetRelativeLocation(FVector(25.0f, 0.0f, 0.0f));
	ProjectileParticle->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
}

void APBProjectile_Fireball::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (DestroyParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestroyParticle, Hit.Location);
	}

	if (DestroySound)
	{
		UE_LOG(LogTemp, Warning, TEXT("SOUNDON"));
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), DestroySound, Hit.Location);
	}
	Destroy();
}