// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PBAttackSkill.h"
#include "PBSkill_DarkBeam.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API APBSkill_DarkBeam : public APBAttackSkill
{
	GENERATED_BODY()
	
public:
	APBSkill_DarkBeam();

protected:
	void BeginPlay() override;

private:
	UFUNCTION()
		void ActiveTimer();

private:
	FTimerHandle ActiveTimerHandle;
};
