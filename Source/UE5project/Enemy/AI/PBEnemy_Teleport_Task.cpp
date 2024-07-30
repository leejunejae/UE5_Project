// Fill out your copyright notice in the Description page of Project Settings.


#include "PBEnemy_Teleport_Task.h"
#include "../PBEnemyAIController.h"
#include "../Human/PBEHuman.h"
#include "BehaviorTree/BlackboardComponent.h"

UPBEnemy_Teleport_Task::UPBEnemy_Teleport_Task()
{
	NodeName = TEXT("Teleport");
	bNotifyTick = true;
}

EBTNodeResult::Type UPBEnemy_Teleport_Task::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<APBEHuman>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// ���� ��ġ�� ������忡 ���� �ϰ� �̵��� ��ġ�� ������ ������
	//FVector TelePortPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector(FName(TEXT("NextPos")));
	
	ControllingPawn->Teleport();
	IsTeleporting = true;

	ControllingPawn->OnTeleport.AddLambda([this]() -> void
		{
			IsTeleporting = false;
		});

	return EBTNodeResult::InProgress;
}

void UPBEnemy_Teleport_Task::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsTeleporting)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}