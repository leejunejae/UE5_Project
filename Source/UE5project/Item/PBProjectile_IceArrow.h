// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PBProjectile.h"
#include "PBProjectile_IceArrow.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API APBProjectile_IceArrow : public APBProjectile
{
	GENERATED_BODY()

public:
	APBProjectile_IceArrow();

protected:
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
};
