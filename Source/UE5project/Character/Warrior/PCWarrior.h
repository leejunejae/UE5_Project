// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../PBCharacter.h"
#include "PCWarrior.generated.h"

/**
 * 
 */

class UPCWAnimInstance;

UCLASS()
class UE5PROJECT_API APCWarrior : public APBCharacter
{
	GENERATED_BODY()
	
public:
	APCWarrior();

	virtual void BeginPlay() override;

	virtual void Death() override;

	virtual void Block(bool CanParried) override;

	virtual void DamageResponse(HitResponse Response) override;

protected:

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

private:
	FTimerHandle MoveSpeedTimerHandle;
	FDamageInfo AttackInfo;

	UPROPERTY(VisibleAnywhere, Category = Animation)
		class UPCWAnimInstance* WarriorAnim;

	UPROPERTY(VisibleAnywhere, Category = Combat)
		class UPBDamageSystem* WarriorDMGSystem;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool CanNextCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsComboInputOn;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 CurrentCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 MaxCombo;



private:
	void MoveSpeedTimer();

	virtual void MoveSpeedToggle() override;
	virtual void Attack() override;

	void AttackStartComboState();
	void AttackEndComboState();
	void AttackTimer();
	void AttackTimerFin();
	void SetAttackInfo(float Amount, AttackType Type, HitResponse Response, bool Invincible = false, bool CanBlocked = false, bool CanParried = false, bool ForceInterrupt = true);

	FTimerHandle AttackTimerHandle;

	FVector PrevLoc;
	FVector NextLoc;
	FVector ABDiff;

	UFUNCTION()
		void IsMontageEnded(UAnimMontage* Montage, bool bInterrupted);
};