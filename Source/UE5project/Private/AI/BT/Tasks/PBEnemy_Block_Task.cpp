// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/Tasks/PBEnemy_Block_Task.h"
#include "Characters/Enemies/PBEnemyAIController.h"
#include "Characters/Enemies/Human/PBEHuman.h"

UPBEnemy_Block_Task::UPBEnemy_Block_Task()
{
	NodeName = TEXT("Block");
	bNotifyTick = true;
}

EBTNodeResult::Type UPBEnemy_Block_Task::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<APBEHuman>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	ControllingPawn->Block(IsDefendMode);

	return EBTNodeResult::Succeeded;
}