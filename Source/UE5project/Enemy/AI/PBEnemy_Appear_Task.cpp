// Fill out your copyright notice in the Description page of Project Settings.


#include "PBEnemy_Appear_Task.h"
#include "../PBEnemyAIController.h"
#include "../Human/PBEHuman.h"

UPBEnemy_Appear_Task::UPBEnemy_Appear_Task()
{
	NodeName = TEXT("Appear");
	bNotifyTick = true;
	IsAppearing = false;
}

EBTNodeResult::Type UPBEnemy_Appear_Task::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<APBEHuman>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	ControllingPawn->Appear();
	IsAppearing = true;
	ControllingPawn->OnAppearEnd.AddLambda([this]() -> void
		{
			IsAppearing = false;
		});

	return EBTNodeResult::InProgress;
}

void UPBEnemy_Appear_Task::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAppearing)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}