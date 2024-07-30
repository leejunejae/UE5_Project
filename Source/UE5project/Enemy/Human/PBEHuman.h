// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../PBEnemy.h"
#include "../../PEnumHeader.h"
#include "../../EnemyHeader.h"
#include "../../Function/Combat/PBDamagableInterface.h"
#include "../../Function/Combat/PBDamageSystem.h"
#include "../../Function/Combat/CombatComponent.h"
#include "../../Function/Combat/CombatInterface.h"
#include "MotionWarpingComponent.h"
#include "Engine/DataTable.h"
#include "PBEHuman.generated.h"

/**
 * 
 */
class UCharacterMovementComponent;
class UPBDamageSystem;

DECLARE_MULTICAST_DELEGATE(FOnActionDelegate);

UCLASS()
class UE5PROJECT_API APBEHuman : public APBEnemy, public IPBDamagableInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	APBEHuman();

	void SetMovementSpeed(float speed);
	FRangeInfo GetIdealRange();

	virtual void TakeDamage_Implementation(FAttackInfo DamageInfo);
	virtual float Heal_Implementation(float amount);
	virtual float GetHealth_Implementation();
	virtual float GetMaxHealth_Implementation();

	virtual UStaticMeshComponent* GetWeapon_Implementation();

	virtual void Attack(FName AttackName, ACharacter* Target = nullptr) override;
	virtual void Summon();
	virtual void Appear();
	virtual void Swoop();
	virtual void Dash(FVector TargetLocation);
	virtual void Block(bool IsDefenseMode);
	UFUNCTION()
		virtual void Death();

	virtual void Teleport();

	FString GetName();
	int32 GetMaxHP();
	int32 GetOffensePower();
	int32 GetDefenseCap();
	int32 GetDownPercent();

	// Focus
	bool IsLookingAt();
	AActor* GetLookingTarget();
	void SetLookingTarget(AActor* Target);
	void SetIsLook(bool isLook);

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	UFUNCTION()
		virtual void IsMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
		virtual void OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	UFUNCTION()
		virtual void OnMontageNotifyEnd(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

public:
	FOnActionDelegate OnTeleport; // ������ ���� �ڷ���Ʈ Behavior Tree Task�� ���� ��������Ʈ
	FOnActionDelegate OnSummonEnd;
	FOnActionDelegate OnAttackEnd;
	FOnActionDelegate OnAttackStart;
	FOnActionDelegate OnAttackFin;
	FOnActionDelegate OnAppearEnd;
	FOnActionDelegate OnSwoopEnd;
	FOnActionDelegate OnBlockEnd;
	FOnActionDelegate OnDashEnd;

protected:
	FEnemyInfo EnemyInfo;

	UPROPERTY(VisibleAnywhere, Category = Combat)
		UPBDamageSystem* DamageSystem;

	UPROPERTY(VisibleAnywhere, Category = Combat)
		UCombatComponent* CombatComponent;

	UPROPERTY(VisibleAnywhere, Category = Combat)
		UMotionWarpingComponent* MotionWarpComp;

	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = true))
		class UDataTable* AttackDT;

	UPROPERTY(VisibleAnywhere, Category = Equipment)
		UStaticMeshComponent* Weapon;

	UPROPERTY(VisibleAnywhere, Category = Equipment)
		UStaticMeshComponent* SubEquip;

	bool IsDead;
	bool IsSummon;
	bool IsAction;
	bool IsAppear;
	bool IsHit;
	bool IsBlock;
	bool IsParried;

	MovementMode CurrentMovement;
	float MeleeRadius;
	float DefendRadius;
	float RangedRadius;

	FTimerHandle AttackTimerHandle;

	bool IsLookAt = false;
	AActor* LookAtTarget;
};