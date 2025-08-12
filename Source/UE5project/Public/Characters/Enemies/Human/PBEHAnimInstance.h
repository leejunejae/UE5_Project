// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PEnumHeader.h"
#include "Characters/Enemies/Data/EnemyData.h"
#include "PBEHAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnStartHitDelegate);
DECLARE_MULTICAST_DELEGATE(FOnEndHitDelegate);
/**
 * 
 */
UCLASS()
class UE5PROJECT_API UPBEHAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPBEHAnimInstance();
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual void PlayMontage(FName MontageName);
	virtual MontageType CheckMontage(UAnimMontage* Montage);

public:
	FOnStartHitDelegate OnStartHit;
	FOnEndHitDelegate OnEndHit;

protected:
	UFUNCTION()
		void AnimNotify_NOT_StartHit();

	UFUNCTION()
		void AnimNotify_NOT_EndHit();

protected:
	class APBEHuman* Character = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* AttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Hit, Meta = (AllowPrivateAccess = true))
		UAnimMontage* DeathMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Hit, Meta = (AllowPrivateAccess = true))
		UAnimMontage* HitMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Montage, Meta = (AllowPrivateAccess = true))
		UAnimMontage* AppearMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Classes, Meta = (AllowPrivateAccess = true))
		class UAnimInstance* AnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		bool IsDead;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		bool IsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		float Direction;
};
