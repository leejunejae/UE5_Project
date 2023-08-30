// Fill out your copyright notice in the Description page of Project Settings.


#include "PBEnemy_CheckRange_Decorator.h"
#include "Enemy/Human/PBEHumanAI.h"
#include "Character/PBCharacter.h"

UPBEnemy_CheckRange_Decorator::UPBEnemy_CheckRange_Decorator()
{
	NodeName = TEXT("CheckRange");
}

bool UPBEnemy_CheckRange_Decorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
		return false;

	auto Target = Cast<APBCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
	if (Target == nullptr)
		return false;
	
	auto Range = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(IdealRange.SelectedKeyName);

	return bResult && (Target->GetDistanceTo(ControllingPawn) - 50.0f <= Range);
}