// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_Dash.h"
#include "../PBEnemyAIController.h"
#include "../Human/PBEHuman.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

UBTTaskNode_Dash::UBTTaskNode_Dash()
{
	NodeName = TEXT("Dash");
	bNotifyTick = true;
	IsDashing = false;
}

EBTNodeResult::Type UBTTaskNode_Dash::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<APBEHuman>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		const FVector DashDirection = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BlackboardKey.SelectedKeyName);
		ControllingPawn->Dash(DashDirection);
	}
	else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		UObject* DashTarget = OwnerComp.GetBlackboardComponent()->GetValueAsObject(BlackboardKey.SelectedKeyName);
		if (DashTarget == nullptr)
		{
			return EBTNodeResult::Failed;
		}
		AActor* TargetActor = Cast<AActor>(DashTarget);
		if (TargetActor == nullptr)
		{
			return EBTNodeResult::Failed;
		}
		const FVector DashDirection = TargetActor->GetActorLocation();
		ControllingPawn->Dash(DashDirection);
	}

	//UE_LOG(LogTemp, Warning, TEXT("%f, %f "), DashDirection.Y, DashDirection.X);
	IsDashing = true;


	ControllingPawn->OnDashEnd.AddLambda([this]() -> void
		{
			IsDashing = false;
		});

	return EBTNodeResult::InProgress;
}

void UBTTaskNode_Dash::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsDashing)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}