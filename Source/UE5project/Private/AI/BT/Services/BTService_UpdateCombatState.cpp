// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/Services/BTService_UpdateCombatState.h"
#include "Characters/Enemies/EnemyBaseAIController.h"
#include "Characters/Enemies/EnemyBase.h"

#include "Utils/CustomMathUtility.h"

using namespace CustomMath::Spatial;

UBTService_UpdateCombatState::UBTService_UpdateCombatState()
{
	NodeName = TEXT("UpdateCombatState");
	bNotifyTick = true;

}

void UBTService_UpdateCombatState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	AEnemyBaseAIController* EnemyController = Cast<AEnemyBaseAIController>(OwnerComp.GetAIOwner());
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if (!EnemyController || !Blackboard) return;

	const UDataTable* CombatPatternDT = EnemyController->GetCombatPatternData();
	AEnemyBase* Enemy = Cast<AEnemyBase>(EnemyController->GetPawn());

	if (!CombatPatternDT || !Enemy) return;

	FCombatContext Context;

	AActor* Target = Cast<AActor>(Blackboard->GetValueAsObject(EnemyController->Key_Target));

	Context.DistanceToTarget = Blackboard->GetValueAsFloat(EnemyController->Key_TargetDistance);
	Context.AbsDeltaYawDeg = AbsDeltaYawDeg(Enemy, Target);
	Context.Aggressiveness; // EnemyStat에서 가져오기
	Context.bHasLOS = true; // 
	Context.bPoiseBroken;
	Context.bRangedThreat;
	Context.bStanceBroken;
	Context.bTargetAttacking;
	Context.bTargetGuarding;
	Context.bTargetInRecovery;
	Context.bTargetPoiseBroken;
	Context.bTargetStanceBroken;
	Context.HPPercent;
	Context.Phase;
	Context.CurrentTime;;

	//CombatPatternDT->
}
