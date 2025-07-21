// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/Tasks/PBEnemy_MagicAttack_Task.h"
#include "Characters/Enemies/PBEnemyAIController.h"
#include "Characters/Enemies/Human/HereticMagician/PBEHHeretic.h"
#include "PEnumHeader.h"

UPBEnemy_MagicAttack_Task::UPBEnemy_MagicAttack_Task()
{
	NodeName = TEXT("MagicAttack");
	bNotifyTick = true;
	IsAttacking = false;
}

EBTNodeResult::Type UPBEnemy_MagicAttack_Task::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<APBEHHeretic>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	HereticSkill SelectedSkill = static_cast<HereticSkill>(OwnerComp.GetBlackboardComponent()->GetValueAsEnum(FName(TEXT("Skill"))));
	ControllingPawn->Skill_Attack(SelectedSkill);
	IsAttacking = true;

	ControllingPawn->OnAttackEnd.AddLambda([this]() -> void
		{
			IsAttacking = false;
		});

	return EBTNodeResult::InProgress;
}

void UPBEnemy_MagicAttack_Task::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
