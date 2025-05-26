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
	void Tick(float DeltaTime) override;
	void Skill_Attack(HereticSkill SelectedSkill);
	void Summon() override;
	void Skill_DarkTornado();
	void Skill_DarkSawTooth();
	void Skill_DarkBall();
	void Skill_DarkSpear();
	void Skill_DarkBeam();
	bool CheckBool(HereticVar CheckVar);
	TArray<HereticSkill> CheckSkillIsReady();

protected:
	void Teleport() override;

private:
	virtual void PostInitializeComponents() override;
	
	void IsMontageEnded(UAnimMontage* Montage, bool bInterrupted) override;

	UFUNCTION()
		void DarkBallTimer();
	UFUNCTION()
		void DarkBeamTimer();
	UFUNCTION()
		void TornadoTimer();


	UFUNCTION()
		void SummonSoldierTimer();
	UFUNCTION()
		void DarkBallCoolTimer();
	UFUNCTION()
		void TornadoCoolTimer();
	UFUNCTION()
		void SawToothCoolTimer();
	UFUNCTION()
		void DarkSpearCoolTimer();
	UFUNCTION()
		void DarkBeamCoolTimer();

private:
	UPROPERTY(VisibleAnywhere, Category = Animation)
		class UPBEMAnimInstance* HereticAnim;

	//Skill Var
	bool IsCasting;
	bool TornadoIsReady;
	bool SawToothIsReady;
	bool DarkBallIsReady;
	bool DarkSpearIsReady;
	bool DarkBeamIsReady;
	bool IsTeleporting;

	bool Tornado;
	bool SawTooth;
	bool DarkBall;
	bool DarkSpear;
	bool DarkBeam;

	int32 NSummon;

	FTimerHandle DarkBallTimerHandle;
	FTimerHandle DarkBeamTimerHandle;
	FTimerHandle TornadoTimerHandle;
	FTimerHandle SummonSoldierTimerHandle;

	FTimerHandle DarkBallCoolTimeHandle;
	FTimerHandle TornadoCoolTimeHandle;
	FTimerHandle SawToothCoolTimeHandle;
	FTimerHandle DarkSpearCoolTimeHandle;
	FTimerHandle DarkBeamCoolTimeHandle;
};
