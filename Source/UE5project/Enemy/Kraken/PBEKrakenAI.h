// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../PBEnemyAIController.h"
#include "PBEKrakenAI.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API APBEKrakenAI : public APBEnemyAIController
{
	GENERATED_BODY()
	
public:
	APBEKrakenAI();
	virtual void OnPossess(APawn* InPawn) override;

	static const FName PrevPos;
	static const FName NextPos;
	static const FName TargetKey;

private:
	UPROPERTY()
		class UBehaviorTree* BTAsset;

	UPROPERTY()
		class UBlackboardData* BBAsset;

	void OnRepeatTimer();
	FTimerHandle RepeatTimerHandle;
	float RepeatInterval;
};
