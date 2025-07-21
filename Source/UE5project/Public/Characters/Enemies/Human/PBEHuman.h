// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemies/PBEnemy.h"

// 구조체, 자료형
#include "PEnumHeader.h"
#include "Characters/Enemies/Data/EnemyHeader.h"

// 인터페이스
#include "Combat/Interfaces/HitReactionInterface.h"
#include "Combat/Interfaces/CombatInterface.h"

// 이동
#include "MotionWarpingComponent.h"

// 데이터테이블
#include "Engine/DataTable.h"
#include "PBEHuman.generated.h"

/**
 * 
 */
class UCharacterMovementComponent;
class UStatComponent;
class UAttackComponent;

DECLARE_MULTICAST_DELEGATE(FOnMultiDelegate);

UCLASS()
class UE5PROJECT_API APBEHuman : public APBEnemy, public IHitReactionInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	APBEHuman();

	void SetMovementSpeed(float speed);
	FRangeInfo GetIdealRange();

	/*
	virtual void TakeDamage_Implementation(FAttackInfo DamageInfo);
	virtual float Heal_Implementation(float amount);
	virtual float GetHealth_Implementation();
	virtual float GetMaxHealth_Implementation();
	*/

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
	FOnMultiDelegate OnTeleport; // ������ ���� �ڷ���Ʈ Behavior Tree Task�� ���� ��������Ʈ
	FOnMultiDelegate OnSummonEnd;
	FOnMultiDelegate OnAttackEnd;
	FOnMultiDelegate OnAttackStart;
	FOnMultiDelegate OnAttackFin;
	FOnMultiDelegate OnAppearEnd;
	FOnMultiDelegate OnSwoopEnd;
	FOnMultiDelegate OnBlockEnd;
	FOnMultiDelegate OnDashEnd;

protected:
	FEnemyInfo EnemyInfo;

	UPROPERTY(VisibleAnywhere, Category = Combat)
		UStatComponent* Statcomponent;

	UPROPERTY(VisibleAnywhere, Category = Combat)
		UAttackComponent* AttackComponent;

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