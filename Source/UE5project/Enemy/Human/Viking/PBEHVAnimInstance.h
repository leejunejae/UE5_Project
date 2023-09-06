// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../PBEHAnimInstance.h"
#include "PBEHVAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API UPBEHVAnimInstance : public UPBEHAnimInstance
{
	GENERATED_BODY()
	
public:
	UPBEHVAnimInstance();
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayMontage(MontageType Type);
	MontageType CheckMontage(UAnimMontage* Montage);

private:
	//class APBEHViking* Character = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* AttackMontage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Hit, Meta = (AllowPrivateAccess = true))
		UAnimMontage* DeathMontage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Hit, Meta = (AllowPrivateAccess = true))
		UAnimMontage* HitMontage;
};
