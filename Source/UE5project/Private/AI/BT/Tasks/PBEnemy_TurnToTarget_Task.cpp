// Fill out your copyright notice in the Description page of Project Settings.
#include "AI/BT/Tasks/PBEnemy_TurnToTarget_Task.h"
#include "Characters/Enemies/EnemyBaseAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/Human/PBEHuman.h"
#include "Utils/CoreLog.h"

UPBEnemy_TurnToTarget_Task::UPBEnemy_TurnToTarget_Task()
{
	NodeName = TEXT("TurnToTarget");
}

EBTNodeResult::Type UPBEnemy_TurnToTarget_Task::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	UE_LOG(Log_AI, Log, TEXT("[EnemyBaseAIController] Turn To Target"));

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
		return EBTNodeResult::Failed;

	auto ControllingAI = OwnerComp.GetAIOwner();
	auto Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
	if (nullptr == Target)
	{
		UE_LOG(Log_AI, Warning, TEXT("[EnemyBaseAIController] Target NULL"));
		return EBTNodeResult::Failed;
	}

	ControllingAI->SetFocus(Target);
	UE_LOG(Log_AI, Log, TEXT("[EnemyBaseAIController] Set Focus"));

	return EBTNodeResult::Succeeded;
}