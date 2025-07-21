// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PBAttackSkill.h"
#include "PBSkill_DarkSpear.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API APBSkill_DarkSpear : public APBAttackSkill
{
	GENERATED_BODY()
	
public:
	APBSkill_DarkSpear();

protected:
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

	UPROPERTY(VisibleAnywhere, Category = Particle)
		class UNiagaraComponent* SpearDOTNiagara;
};
