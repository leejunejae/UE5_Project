// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/Tasks/PBEnemy_Swoop_Task.h"
#include "Characters/Enemies/PBEnemyAIController.h"
#include "Characters/Enemies/Human/PBEHuman.h"

UPBEnemy_Swoop_Task::UPBEnemy_Swoop_Task()
{
	NodeName = TEXT("Swoop");
	bNotifyTick = true;
	IsSwooping = false;
}

EBTNodeResult::Type UPBEnemy_Swoop_Task::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<APBEHuman>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	ControllingPawn->Swoop();
	IsSwooping = true;
	ControllingPawn->OnSwoopEnd.AddLambda([this]() -> void
		{
			IsSwooping = false;
		});

	return EBTNodeResult::InProgress;
}

void UPBEnemy_Swoop_Task::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsSwooping)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}