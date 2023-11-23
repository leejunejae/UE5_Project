// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../PBEnemy.h"
#include "../../PEnumHeader.h"
#include "../../Function/PBDamagableInterface.h"
#include "../../Function/PBDamageSystem.h"
#include "PBEHuman.generated.h"

/**
 * 
 */
class UCharacterMovementComponent;
class UPBDamageSystem;

DECLARE_MULTICAST_DELEGATE(FOnActionDelegate);

UCLASS()
class UE5PROJECT_API APBEHuman : public APBEnemy, public IPBDamagableInterface
{
	GENERATED_BODY()
	
public:
	APBEHuman();

	void SetMovementSpeed(float speed);
	FRangeInfo GetIdealRange();

	virtual bool TakeDamage_Implementation(FDamageInfo DamageInfo) override;
	virtual float Heal_Implementation(float amount) override;
	virtual float GetHealth_Implementation() override;
	virtual float GetMaxHealth_Implementation() override;

	virtual void Attack() override;
	virtual void Summon();
	virtual void Appear();
	virtual void Swoop();
	UFUNCTION()
		virtual void Death();
	UFUNCTION()
		virtual void Block(bool CanParried);
	UFUNCTION()
		virtual void DamageResponse(HitResponse Response);

	virtual void Teleport();

private:
	FDamageInfo AttackInfo;

protected:
	virtual void PostInitializeComponents() override;

	UFUNCTION()
		virtual void IsMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void AttackTimer();
	void SetAttackInfo(float Amount, AttackType Type, HitResponse Response, bool Invincible = false, bool CanBlocked = false, bool CanParried = false, bool ForceInterrupt = true);

public:
	FOnActionDelegate OnTeleport; // 마법사 적의 텔레포트 Behavior Tree Task를 위한 델리게이트
	FOnActionDelegate OnSummonEnd;
	FOnActionDelegate OnAttackEnd;
	FOnActionDelegate OnAppearEnd;
	FOnActionDelegate OnSwoopEnd;

protected:
	UPROPERTY(VisibleAnywhere, Category = Combat)
		UPBDamageSystem* DamageSystem;

	UPROPERTY(VisibleAnywhere, Category = Equipment)
		UStaticMeshComponent* Weapon;

	UPROPERTY(VisibleAnywhere, Category = Equipment)
		UStaticMeshComponent* SubEquip;

	bool IsDead;
	bool IsSummon;
	bool IsAction;
	bool IsAppear;

	MovementMode CurrentMovement;
	float MeleeRadius;
	float DefendRadius;
	float RangedRadius;

	FTimerHandle AttackTimerHandle;
};
