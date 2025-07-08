// Fill out your copyright notice in the Description page of Project Settings.
#include "Skills/Projectiles/PBProjectile_Darkball.h"

APBProjectile_Darkball::APBProjectile_Darkball()
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PARTICLE(TEXT("/Game/ParagonGideon/FX/Particles/Gideon/Abilities/Burden/FX/P_Gideon_Burden_Projectile.P_Gideon_Burden_Projectile"));
	if (PARTICLE.Succeeded())
	{
		ProjectileParticle->SetTemplate(PARTICLE.Object);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> SPAWNPART(TEXT("/Game/ParagonGideon/FX/Particles/Gideon/Abilities/Burden/FX/P_Gideon_Burden_HitWorld.P_Gideon_Burden_HitWorld"));
	if (SPAWNPART.Succeeded())
	{
		SpawnParticle = SPAWNPART.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> DESTPART(TEXT("/Game/ParagonGideon/FX/Particles/Gideon/Abilities/Burden/FX/P_Gideon_Burden_HitWorld.P_Gideon_Burden_HitWorld"));
	if (DESTPART.Succeeded())
	{
		DestroyParticle=DESTPART.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> DESTSOUND(TEXT("/Game/StarterContent/Audio/Explosion_Cue.Explosion_Cue"));
	if (DESTPART.Succeeded())
	{
		DestroySound = DESTSOUND.Object;
	}

	ProjectileCol->SetBoxExtent(FVector(66.0f, 30.0f, 30.0f));
	ProjectileParticle->SetRelativeLocation(FVector(35.0f, 0.0f, 0.0f));
	ProjectileParticle->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	Tags.Add("Homing");
}

void APBProjectile_Darkball::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
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

void APBProjectile_Darkball::RotateToTarget()
{

}