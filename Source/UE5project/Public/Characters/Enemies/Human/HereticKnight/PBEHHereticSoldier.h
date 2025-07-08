// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemies/Human/PBEHuman.h"
#include "Components/TimeLineComponent.h"
#include "PBEHHereticSoldier.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API APBEHHereticSoldier : public APBEHuman
{
	GENERATED_BODY()
	
public:
	APBEHHereticSoldier();
	void Attack(FName AttackName, ACharacter* Target) override;
	void Appear() override;
	void Death() override;
	void Block(bool IsDefenseMode) override;
	void Dash(FVector TargetLocation) override;
	void SetHSoldierMode(HSoldierMode NextMode);
	bool CheckBool(HSoldierVar CheckVar);
	bool IsDashing();

	HSoldierSkill CheckSkill();
	int32 CheckCombo();
	FVector GetDashDirection();

	void Tick(float DeltaTime) override;

public:
	FOnMultiDelegate OnSwitchEnd;

protected:
	void BeginPlay() override;

	void OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload) override;
	void OnMontageNotifyEnd(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload) override;
		

private:
	UPROPERTY(VisibleAnywhere, Category = Animation)
		class UPBEHHSAnimInstance* HSoldierAnim;

	UPROPERTY(VisibleAnywhere, Category = Particle)
		class UNiagaraComponent* HSoldierAura;

	UPROPERTY(VisibleAnywhere, Category = Particle)
		class UNiagaraComponent* HSoldierDissolve;

	UPROPERTY(VisibleAnywhere, Category = Particle)
		class UNiagaraComponent* HSoldierSpawn;

	UPROPERTY(VisibleAnywhere, Category = Particle)
		UMaterialParameterCollection* DisORSpawnMPC;

	UPROPERTY(VisibleAnywhere, Category = Particle)
		UMaterialParameterCollectionInstance* DisORSpawnMPCInst;


// Dissolve�� ���� Timeline ������
	UPROPERTY(VisibleAnywhere, Category = Timeline)
		UCurveFloat* DisORSpawnCurve;

	FTimeline DisORSpawnTimeline;
	float DisORSpawnTimelineLength;

	UFUNCTION()
		void DisORSpawnUpdate(float Value);

	UFUNCTION()
		void DisORSpawnFin();

private:
	virtual void PostInitializeComponents() override;
	void IsMontageEnded(UAnimMontage* Montage, bool bInterrupted) override;
	void ResetCombo();

private:
	bool IsSwoop;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 CurrentCombo = 0;

	HSoldierSkill CurSkill =  HSoldierSkill::None;
	bool IsBlockBreak, IsHitLarge;
	bool IsCharge;
	bool CanAttack;
	bool IsDefenseMode;
	bool IsDash;

	float AnimPlayRate = 1.0f;
	FVector DashVector;

	HSoldierMode CurMode = HSoldierMode::OffenseMode;


	//Dash Function
	FTimerHandle DashTimerHandle;
	FTimerDelegate DashTimerDelegate;
	void SetDashDirection(FVector TargetLocation);

	UPROPERTY(VisibleAnywhere, Category = Timeline)
		UCurveFloat* DashCurve;

	FVector DashDirection;
	FVector InitPosition;
	FTimeline DashTimeline;
	float DashTimelineLength;
	float PreviousCurveValue;

	UFUNCTION()
		void DashUpdate(float Value);

	UFUNCTION()
		void DashUpdateFin();
	//Dash Function

protected:
	FTimerHandle ChargeTimerHandle;
};
