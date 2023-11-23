// Fill out your copyright notice in the Description page of Project Settings.


#include "PBEnemy_Destroy_Task.h"
#include "../PBEnemyAIController.h"
#include "../Human/PBEHuman.h"

UPBEnemy_Destroy_Task::UPBEnemy_Destroy_Task()
{
	NodeName = TEXT("Destroy");
}

EBTNodeResult::Type UPBEnemy_Destroy_Task::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<APBEHuman>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	ControllingPawn->Death();

	return EBTNodeResult::Succeeded;
}