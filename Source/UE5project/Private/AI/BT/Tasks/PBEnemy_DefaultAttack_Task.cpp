// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/Tasks/PBEnemy_DefaultAttack_Task.h"
#include "Characters/Enemies/EnemyBaseAIController.h"
#include "Characters/Enemies/Human/PBEHuman.h"

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

	auto ControllingAI = OwnerComp.GetAIOwner();
	ControllingAI->ClearFocus(EAIFocusPriority::Gameplay);
	
	auto Target = Cast<ACharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));

	ControllingPawn->Attack(AttackName, Target);
	
	IsAttacking = true;

	ControllingPawn->OnAttackEnd.AddLambda([this]() -> void
		{
			IsFocusing = true;
			ChangeFocus = true;
		});

	ControllingPawn->OnAttackStart.AddLambda([this]() -> void
		{
			IsFocusing = false;
			ChangeFocus = true;
		});

	ControllingPawn->OnAttackFin.AddLambda([this]() -> void
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

	if (ChangeFocus)
	{
		if (IsFocusing)
		{
			auto Target = Cast<ACharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
			OwnerComp.GetAIOwner()->SetFocus(Target);
			UE_LOG(LogTemp, Warning, TEXT("SetFocus"));
		}
		else
		{
			OwnerComp.GetAIOwner()->ClearFocus(EAIFocusPriority::Gameplay);
			UE_LOG(LogTemp, Warning, TEXT("ClearFocus"));
		}
		ChangeFocus = false;
	}
}