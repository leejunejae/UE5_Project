// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PBAttackSkill.h"
#include "PBSkill_DarkTornado.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API APBSkill_DarkTornado : public APBAttackSkill
{
	GENERATED_BODY()
	
public:
	APBSkill_DarkTornado();

protected:
	void BeginPlay() override;

private:
	UFUNCTION()
		void ActiveTimer();

private:
	FTimerHandle ActiveTimerHandle;
};
