// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/Tasks/PBEnemy_Summon_Task.h"
#include "Characters/Enemies/PBEnemyAIController.h"
#include "Characters/Enemies/Human/PBEHuman.h"

UPBEnemy_Summon_Task::UPBEnemy_Summon_Task()
{
	NodeName = TEXT("Summon");
	bNotifyTick = true;
	IsSummoning = false;
}

EBTNodeResult::Type UPBEnemy_Summon_Task::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<APBEHuman>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	ControllingPawn->Summon();
	IsSummoning = true;

	ControllingPawn->OnSummonEnd.AddLambda([this]() -> void
		{
			IsSummoning = false;
		});

	return EBTNodeResult::InProgress;
}

void UPBEnemy_Summon_Task::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsSummoning)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}