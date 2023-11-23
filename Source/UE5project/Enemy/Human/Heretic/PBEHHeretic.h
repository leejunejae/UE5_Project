// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../PBEHuman.h"
#include "PBEHHeretic.generated.h"

/**
 * 
 */
class UPBEMAnimInstance;

UCLASS()
class UE5PROJECT_API APBEHHeretic : public APBEHuman
{
	GENERATED_BODY()
	
public:
	APBEHHeretic();
	void Attack() override;
	void Summon() override;
	void Skill_DarkTornado();
	void Skill_DarkSawTooth();
	void Skill_DarkBall();
	bool CheckBool(HereticVar CheckVar);

protected:
	void Teleport() override;

private:
	virtual void PostInitializeComponents() override;
	
	void IsMontageEnded(UAnimMontage* Montage, bool bInterrupted) override;
	UFUNCTION()
		void DarkballTimer();
	UFUNCTION()
		void SummonSoldierTimer();

private:
	UPROPERTY(VisibleAnywhere, Category = Animation)
		class UPBEMAnimInstance* HereticAnim;

	bool IsTeleporting;
	int32 NSummon;
	FTimerHandle DarkballTimerHandle;
	FTimerHandle SummonSoldierTimerHandle;
};
