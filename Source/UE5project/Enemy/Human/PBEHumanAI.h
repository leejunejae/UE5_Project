// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../PBEnemyAIController.h"
#include "PBEHumanAI.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API APBEHumanAI : public APBEnemyAIController
{
	GENERATED_BODY()
	
public:
	APBEHumanAI();
	virtual void OnPossess(APawn* InPawn) override;

private:
	void OnRepeatTimer();
	FTimerHandle RepeatTimerHandle;
	float RepeatInterval;
};
