// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PBProjectile.h"
#include "PBProjectile_Fireball.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API APBProjectile_Fireball : public APBProjectile
{
	GENERATED_BODY()
	
public:
	APBProjectile_Fireball();

protected:
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
};
