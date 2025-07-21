// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/Kraken/PBEKraken_Patrol_Task.h"
#include "Characters/Enemies/PBEnemyAIController.h"
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

	// ���� ��ġ�� ������忡 ���� �ϰ� �̵��� ��ġ�� ������ ������
	FVector PrevPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector(FName(TEXT("PrevPos")));
	FNavLocation NextLocation;

	// ���� ��ġ���� 1500f�ȿ� �ִ� �������� �����ϱ� �̵�
	if (NavSystem->GetRandomPointInNavigableRadius(PrevPos, 1500.0f, NextLocation))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName(TEXT("NextPos")), NextLocation.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}