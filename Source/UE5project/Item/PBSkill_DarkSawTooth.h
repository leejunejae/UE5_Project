// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PBAttackSkill.h"
#include "PBSkill_DarkSawTooth.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API APBSkill_DarkSawTooth : public APBAttackSkill
{
	GENERATED_BODY()
	
public:
	APBSkill_DarkSawTooth();

protected:
	void BeginPlay() override;
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

private:
	UFUNCTION()
		void ReturnTimer();

private:
	FTimerHandle ReturnTimerHandle;
};
