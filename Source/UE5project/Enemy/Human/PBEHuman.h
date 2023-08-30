// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../PBEnemy.h"
#include "../../PEnumHeader.h"
#include "../../PBDamagableInterface.h"
#include "../../PBDamageSystem.h"
#include "PBEHuman.generated.h"

/**
 * 
 */
class UCharacterMovementComponent;
class UPBDamageSystem;

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

	UFUNCTION()
		virtual void Death();
	UFUNCTION()
		virtual void Block(bool CanParried);
	UFUNCTION()
		virtual void DamageResponse(HitResponse Response);

private:
	

protected:
	virtual void PostInitializeComponents() override;

	virtual void Attack() override;


protected:
	UPROPERTY(VisibleAnywhere, Category = Combat)
		UPBDamageSystem* DamageSystem;

	UPROPERTY(VisibleAnywhere, Category = Equipment)
		UStaticMeshComponent* Weapon;

	UPROPERTY(VisibleAnywhere, Category = Equipment)
		UStaticMeshComponent* SubEquip;

	MovementMode CurrentMovement;
	float MeleeRadius;
	float DefendRadius;
	float RangedRadius;
};
