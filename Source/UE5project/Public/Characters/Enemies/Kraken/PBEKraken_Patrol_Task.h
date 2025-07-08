// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "PBEKraken_Patrol_Task.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API UPBEKraken_Patrol_Task : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UPBEKraken_Patrol_Task();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
