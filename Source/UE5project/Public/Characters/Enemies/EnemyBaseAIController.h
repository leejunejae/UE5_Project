// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "EnemyBaseAIController.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API AEnemyBaseAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AEnemyBaseAIController();
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	void SetControllerData(UBehaviorTree* BehaviorTree, UBlackboardData* Blackboard);
	//virtual void OnUnPossess() override;


public:
	static const FName PrevPos;
	static const FName NextPos;
	static const FName TargetKey;
	static const FName MeleeRadius;
	static const FName RangedRadius;
	static const FName DefendRadius;

	AActor* AttackTarget;

protected:
	UPROPERTY(VisibleAnywhere, Category = AI)
		class UBehaviorTree* CachedBT;

	UPROPERTY(VisibleAnywhere, Category = AI)
		class UBlackboardData* CachedBB;

	UFUNCTION()
		void OnTargetPerceptionUpdated_Delegate(AActor* Actor, FAIStimulus Stimulus);

	UPROPERTY(VisibleAnywhere, Category = AI)
		TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent = nullptr;

	UPROPERTY(VisibleAnywhere, Category = AI)
		TObjectPtr<class UAISenseConfig_Sight> AISenseConfigSight = nullptr;


	//void OnRepeatTimer();

	//FTimerHandle RepeatTimerHandle;
	//float RepeatInterval;
};
