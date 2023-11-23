// Fill out your copyright notice in the Description page of Project Settings.


#include "PBEKraken_Patrol_Task.h"
#include "../PBEnemyAIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UPBEKraken_Patrol_Task::UPBEKraken_Patrol_Task()
{
	NodeName = TEXT("FindPos");
}

EBTNodeResult::Type UPBEKraken_Patrol_Task::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (NavSystem == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// 현재 위치를 블랙보드에 저장 하고 이동할 위치를 저장할 변수들
	FVector PrevPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector(FName(TEXT("PrevPos")));
	FNavLocation NextLocation;

	// 현재 위치에서 1500f안에 있는 지점으로 랜덤하기 이동
	if (NavSystem->GetRandomPointInNavigableRadius(PrevPos, 1500.0f, NextLocation))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName(TEXT("NextPos")), NextLocation.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}