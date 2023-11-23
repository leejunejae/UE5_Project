// Fill out your copyright notice in the Description page of Project Settings.


#include "PBEnemy_DefaultAttack_Task.h"
#include "../PBEnemyAIController.h"
#include "../Human/PBEHuman.h"

UPBEnemy_DefaultAttack_Task::UPBEnemy_DefaultAttack_Task()
{
	NodeName = TEXT("DefaultAttack");
	bNotifyTick = true;
	IsAttacking = false;
}

EBTNodeResult::Type UPBEnemy_DefaultAttack_Task::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<APBEHuman>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	ControllingPawn->Attack();
	IsAttacking = true;

	ControllingPawn->OnAttackEnd.AddLambda([this]() -> void
		{
			IsAttacking = false;
		});

	return EBTNodeResult::InProgress;
}

void UPBEnemy_DefaultAttack_Task::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}