// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/Tasks/PBEnemy_SetMovementSpeed_Task.h"
#include "Characters/Enemies/Human/PBEHumanAI.h"
#include "Characters/Enemies/Human/PBEHuman.h"


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