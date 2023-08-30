// Fill out your copyright notice in the Description page of Project Settings.


#include "PBEnemy_SetMovementSpeed_Task.h"
#include "Enemy/Human/PBEHumanAI.h"
#include "Enemy/Human/PBEHuman.h"


UPBEnemy_SetMovementSpeed_Task::UPBEnemy_SetMovementSpeed_Task()
{
	NodeName = TEXT("SetMovementSpeed");
}

EBTNodeResult::Type UPBEnemy_SetMovementSpeed_Task::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<APBEHumanAI>(OwnerComp.GetAIOwner());
	if (nullptr == ControllingPawn)
		return EBTNodeResult::Failed;

	ControllingPawn->SetMovementMode(CurrentMovement);
	return EBTNodeResult::Succeeded;
}