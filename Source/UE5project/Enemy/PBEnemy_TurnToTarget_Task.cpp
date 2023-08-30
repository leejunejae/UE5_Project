// Fill out your copyright notice in the Description page of Project Settings.


#include "PBEnemy_TurnToTarget_Task.h"
#include "PBEnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../Character/PBCharacter.h"

UPBEnemy_TurnToTarget_Task::UPBEnemy_TurnToTarget_Task()
{
	NodeName = TEXT("TurnToTarget");
}

EBTNodeResult::Type UPBEnemy_TurnToTarget_Task::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
		return EBTNodeResult::Failed;

	auto ControllingAI = OwnerComp.GetAIOwner();
	auto Target = Cast<APBCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
	if (nullptr == Target)
		return EBTNodeResult::Failed;

	ControllingAI->SetFocus(Target);

	return EBTNodeResult::Succeeded;
}