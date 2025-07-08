// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "PBEnemy_Teleport_Task.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API UPBEnemy_Teleport_Task : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UPBEnemy_Teleport_Task();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	bool IsTeleporting;
};
