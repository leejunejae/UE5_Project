// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PBProjectile.generated.h"

class UProjectileMovementComponent;
class UStaticMeshComponent;
class UBoxComponent;
class UArrowComponent;
class UParticleSystemComponent;

UCLASS()
class UE5PROJECT_API APBProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APBProjectile();

protected:
	UPROPERTY(VisibleAnywhere, Category = Projectile)
		UBoxComponent* ProjectileCol;

	UPROPERTY(VisibleAnywhere, Category = Projectile)
		UArrowComponent* ProjectileArrow;

	UPROPERTY(VisibleAnywhere, Category = Projectile)
		UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleAnywhere, Category = Projectile)
		UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, Category = Projectile)
		UParticleSystemComponent* ProjectileParticle;

	UPROPERTY(VisibleAnywhere, Category = Projectile)
		UParticleSystem* DestroyParticle;

	UPROPERTY(VisibleAnywhere, Category = Projectile)
		UParticleSystem* SpawnParticle;

	UPROPERTY(VisibleAnywhere, Category = Projectile)
		USoundCue* DestroySound;

	UPROPERTY(EditAnywhere, Category = Projectile)
		float Speed = 500.0f;

	UPROPERTY(EditAnywhere, Category = Projectile)
		float Gravity = 0.0f;

	UPROPERTY(EditAnywhere, Category = Projectile)
		ACharacter* Target;
	
		//FMath::FRandRange(0.0f,1.0f);

	FTimerHandle SpawnTimerHandle;
	int32 SpawnCount = 0;

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void RotateToTarget();

	UFUNCTION()
		virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
