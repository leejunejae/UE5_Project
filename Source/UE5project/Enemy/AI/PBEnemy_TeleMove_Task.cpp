// Fill out your copyright notice in the Description page of Project Settings.


#include "PBEnemy_TeleMove_Task.h"
#include "../PBEnemyAIController.h"
#include "../../Item/PBSkill_TeleportFin.h"
#include "BehaviorTree/BlackboardComponent.h"

UPBEnemy_TeleMove_Task::UPBEnemy_TeleMove_Task()
{
	NodeName = TEXT("TeleMove");
}

EBTNodeResult::Type UPBEnemy_TeleMove_Task::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// 현재 위치를 블랙보드에 저장 하고 이동할 위치를 저장할 변수들
	FVector TelePortPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector(FName(TEXT("NextPos")));

	ControllingPawn->SetActorLocation(TelePortPos);
	FVector TeleLocation = TelePortPos + ControllingPawn->GetActorUpVector() * 90.0f + ControllingPawn->GetActorForwardVector() * -10.0f;
	APBSkill_TeleportFin* TeleportFin = GetWorld()->SpawnActor<APBSkill_TeleportFin>(TeleLocation, ControllingPawn->GetActorRotation());
	return EBTNodeResult::Succeeded;
}