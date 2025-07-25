// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemies/EnemyBaseAIController.h"
#include "PEnumHeader.h"
#include "PBEHumanAI.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API APBEHumanAI : public AEnemyBaseAIController
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

	void SetMovementMode(float MovementSpeed);
	virtual void StopAI();
	//virtual void RunAI();

	virtual void UpdateControlRotation(float DeltaTime, bool bUpdatePawn = true) override;


protected:
	virtual void BeginPlay() override;

protected:
	FRotator SmoothTargetRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SmoothFocusInterpSpeed = 360.0f;

private:
	void OnRepeatTimer();
	FTimerHandle RepeatTimerHandle;
	float RepeatInterval;
};
