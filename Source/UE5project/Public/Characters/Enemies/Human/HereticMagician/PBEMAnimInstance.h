// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PEnumHeader.h"
#include "PBEMAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnEndSkillDelegate);
DECLARE_MULTICAST_DELEGATE(FOnStartSkillDelgeate);
/**
 * 
 */
UCLASS()
class UE5PROJECT_API UPBEMAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPBEMAnimInstance();
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayMontage(FName MontageName);
	HereticMontage CheckMontage(UAnimMontage* Montage);

public:
	FOnStartSkillDelgeate OnDarkBallStart;
	FOnStartSkillDelgeate OnSawToothStart;
	FOnStartSkillDelgeate OnDarkSpearStart;
	FOnStartSkillDelgeate OnDarkBeamStart;
	FOnStartSkillDelgeate OnTornadoStart;

	FOnEndSkillDelegate OnDarkBallEnd;
	FOnEndSkillDelegate OnSawToothEnd;
	FOnEndSkillDelegate OnDarkSpearEnd;
	FOnEndSkillDelegate OnDarkBeamEnd;
	FOnEndSkillDelegate OnTornadoEnd;

protected:
	class APBEHHeretic* Character = nullptr;

private:
	UFUNCTION()
		void AnimNotify_NOT_Darkball();

	UFUNCTION()
		void AnimNotify_NOT_DarkballEnd();

	UFUNCTION()
		void AnimNotify_NOT_SawTooth();

	UFUNCTION()
		void AnimNotify_NOT_SawToothEnd();

	UFUNCTION()
		void AnimNotify_NOT_DarkSpear();

	UFUNCTION()
		void AnimNotify_NOT_DarkSpearEnd();

	UFUNCTION()
		void AnimNotify_NOT_DarkBeam();

	UFUNCTION()
		void AnimNotify_NOT_DarkBeamEnd();

	UFUNCTION()
		void AnimNotify_NOT_Tornado();

	UFUNCTION()
		void AnimNotify_NOT_TornadoEnd();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		bool IsSummon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		bool Tornado;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		bool SawTooth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		bool DarkBall;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		bool DarkSpear;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		bool DarkBeam;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
		UAnimMontage* TeleportMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
		UAnimMontage* FireProjMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
		UAnimMontage* SummonMontage;
};