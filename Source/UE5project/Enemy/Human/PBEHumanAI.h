// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../PBEnemyAIController.h"
#include "../../PEnumHeader.h"
#include "PBEHumanAI.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API APBEHumanAI : public APBEnemyAIController
{
	GENERATED_BODY()
	
public:
	APBEHumanAI();
	virtual void OnPossess(APawn* InPawn) override;

	AActor* AttackTarget;

	UFUNCTION()
		void OnTargetPerceptionUpdated_Delegate(AActor* Actor, FAIStimulus Stimulus);

	UPROPERTY(VisibleAnywhere, Category = AI)
		TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent = nullptr;

	UPROPERTY(VisibleAnywhere, Category = AI)
		TObjectPtr<class UAISenseConfig_Sight> AISenseConfigSight = nullptr;

	void SetMovementMode(MovementMode mode);
	//virtual void RunAI();

protected:
	virtual void BeginPlay() override;

protected:

private:
	void OnRepeatTimer();
	FTimerHandle RepeatTimerHandle;
	float RepeatInterval;
};
