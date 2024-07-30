// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PBSkill.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "PBAttackSkill.generated.h"

class UProjectileMovementComponent;

UCLASS()
class UE5PROJECT_API APBAttackSkill : public APBSkill
{
	GENERATED_BODY()

public:
	APBAttackSkill();
	
protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	UFUNCTION()
		virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	UPROPERTY(VisibleAnywhere, Category = Skill)
		UProjectileMovementComponent* SkillMovement;

	UPROPERTY(EditAnywhere, Category = Skill)
		ACharacter* Target;

	UPROPERTY(EditAnywhere, Category = Projectile)
		float Speed = 1000.0f;
private:
};
