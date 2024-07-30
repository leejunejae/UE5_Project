// Fill out your copyright notice in the Description page of Project Settings.


#include "PBHSoldier_SelectMode_Task.h"
#include "../Human/Heretic/PBEHHereticSoldierAI.h"
#include "../Human/Heretic/PBEHHereticSoldier.h"
#include "BehaviorTree/BlackboardComponent.h"

UPBHSoldier_SelectMode_Task::UPBHSoldier_SelectMode_Task()
{
	NodeName = TEXT("SelectMode");
	Stance = HSoldierMode::OffenseMode;
	bNotifyTick = true;
	SwitchMode = false;
}

EBTNodeResult::Type UPBHSoldier_SelectMode_Task::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<APBEHHereticSoldier>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	int32 SS_Probability = FMath::RandRange(0, 100);

	if (SS_Probability > 80)
	{
		if (Stance == HSoldierMode::OffenseMode)
			Stance = HSoldierMode::DefenseMode;
		else
			Stance = HSoldierMode::OffenseMode;
	}
	else
		return EBTNodeResult::Succeeded;

	ControllingPawn->SetHSoldierMode(Stance);
	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("HSoldierMode")), static_cast<uint8>(Stance));
	SwitchMode = true;

	ControllingPawn->OnSwitchEnd.AddLambda([this]() -> void
		{
			SwitchMode = false;
		});

	return EBTNodeResult::InProgress;
}

void UPBHSoldier_SelectMode_Task::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!SwitchMode)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}