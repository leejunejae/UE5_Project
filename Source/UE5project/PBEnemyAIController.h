// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
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
	virtual void OnPossess(APawn* InPawn) override;
	//virtual void OnUnPossess() override;

	UPROPERTY()
		class UBehaviorTree* BTAsset;

	UPROPERTY()
		class UBlackboardData* BBAsset;
};
