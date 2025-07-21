// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTaskNode_Dash.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API UBTTaskNode_Dash : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTaskNode_Dash();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	bool IsDashing = false;
};
