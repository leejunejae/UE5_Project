// Fill out your copyright notice in the Description page of Project Settings.


#include "PBEHOrcAI.h"
#include "PBEHOrc.h"

APBEHOrcAI::APBEHOrcAI()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/Enemy/E_Human/EH_Viking/EHV_AI/EHV_BlackBoard.EHV_BlackBoard"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/Enemy/E_Human/EH_Viking/EHV_AI/EHV_BehaviorTree.EHV_BehaviorTree"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}

void APBEHOrcAI::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	auto ControllingPawn = Cast<APBEHOrc>(GetPawn());
	Blackboard->SetValueAsFloat(MeleeRadius, ControllingPawn->GetIdealRange().Melee);
	Blackboard->SetValueAsFloat(RangedRadius, ControllingPawn->GetIdealRange().Ranged);
	Blackboard->SetValueAsFloat(DefendRadius, ControllingPawn->GetIdealRange().Defend);
}

void APBEHOrcAI::StopAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BehaviorTreeComponent && BehaviorTreeComponent->IsRunning())
		BehaviorTreeComponent->StopTree();
}