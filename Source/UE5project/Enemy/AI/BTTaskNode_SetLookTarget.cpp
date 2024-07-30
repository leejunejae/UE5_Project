// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_SetLookTarget.h"
#include "../PBEnemyAIController.h"
#include "../Human/PBEHuman.h"

UBTTaskNode_SetLookTarget::UBTTaskNode_SetLookTarget()
{
	NodeName = TEXT("SetLookTarget");
}

EBTNodeResult::Type UBTTaskNode_SetLookTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<APBEHuman>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
	if (Target == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	ControllingPawn->SetLookingTarget(Target);

	return EBTNodeResult::Succeeded;
}