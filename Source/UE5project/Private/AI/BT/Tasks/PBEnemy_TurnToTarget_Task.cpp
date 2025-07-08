// Fill out your copyright notice in the Description page of Project Settings.
#include "AI/BT/Tasks/PBEnemy_TurnToTarget_Task.h"
#include "Characters/Enemies/PBEnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/Human/PBEHuman.h"

UPBEnemy_TurnToTarget_Task::UPBEnemy_TurnToTarget_Task()
{
	NodeName = TEXT("TurnToTarget");
}

EBTNodeResult::Type UPBEnemy_TurnToTarget_Task::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<APBEHuman>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
		return EBTNodeResult::Failed;

	auto ControllingAI = OwnerComp.GetAIOwner();
	auto Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
	if (nullptr == Target)
		return EBTNodeResult::Failed;
	
	if (IsMove)
	{
		ControllingAI->SetFocus(Target);
		//ControllingPawn->bUseControllerRotationYaw;
	}
	else
	{
		ControllingPawn->SetLookingTarget(Target);
		ControllingPawn->SetIsLook(true);
	}
	return EBTNodeResult::Succeeded;
}