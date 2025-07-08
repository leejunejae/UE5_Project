// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "PBEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API APBEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	APBEnemyAIController();
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	//virtual void OnUnPossess() override;


public:
	static const FName PrevPos;
	static const FName NextPos;
	static const FName TargetKey;
	static const FName MeleeRadius;
	static const FName RangedRadius;
	static const FName DefendRadius;

protected:
	UPROPERTY()
		class UBehaviorTree* BTAsset;

	UPROPERTY()
		class UBlackboardData* BBAsset;
	//void OnRepeatTimer();

	//FTimerHandle RepeatTimerHandle;
	//float RepeatInterval;
};
