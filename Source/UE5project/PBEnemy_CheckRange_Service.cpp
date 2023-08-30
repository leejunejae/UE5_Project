// Fill out your copyright notice in the Description page of Project Settings.


#include "PBEnemy_CheckRange_Service.h"
#include "Enemy/Human/PBEHumanAI.h"
#include "Character/PBCharacter.h"

UPBEnemy_CheckRange_Service::UPBEnemy_CheckRange_Service()
{
	NodeName = TEXT("CheckRange");
	Interval = 1.0f;
}

void UPBEnemy_CheckRange_Service::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
		return;

	auto Target = Cast<APBCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
	if (Target == nullptr)
		return;

	auto Range = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(IdealRange.SelectedKeyName);

	if (Target->GetDistanceTo(ControllingPawn) - 50.0f <= Range)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName(TEXT("IsIdealRange")), true);
		return;
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName(TEXT("IsIdealRange")), false);
		return;
	}
}