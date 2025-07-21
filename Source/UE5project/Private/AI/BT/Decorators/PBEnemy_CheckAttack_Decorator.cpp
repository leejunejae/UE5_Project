// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/Decorators/PBEnemy_CheckAttack_Decorator.h"
#include "Characters/Player/Warrior/FallenKnight.h"
#include "Characters/Enemies/Human/PBEHumanAI.h"
#include "BehaviorTree/BlackboardComponent.h"

UPBEnemy_CheckAttack_Decorator::UPBEnemy_CheckAttack_Decorator()
{
	NodeName = TEXT("CheckAttack");
}

bool UPBEnemy_CheckAttack_Decorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
		return false;

	auto Target = Cast<AFallenKnight>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
	if (Target == nullptr)
	{
		return false;
	}

	return Target->IsAttacking() && Target->GetDistanceTo(ControllingPawn) < 300.0f;
}