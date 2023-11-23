// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../PBEHAnimInstance.h"
#include "PBEHHSAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnMultDelegate);
/**
 * 
 */
UCLASS()
class UE5PROJECT_API UPBEHHSAnimInstance : public UPBEHAnimInstance
{
	GENERATED_BODY()
	
public:
	UPBEHHSAnimInstance();
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	void PlayMontage(MontageType Type) override;
	MontageType CheckMontage(UAnimMontage* Montage) override;

public:
	FOnMultDelegate OnRush;
	FOnMultDelegate OnTeleport;

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Montage, Meta = (AllowPrivateAccess = true))
		UAnimMontage* SwoopMontage;

private:
	UFUNCTION()
		void AnimNotify_NOT_Rush();

	UFUNCTION()
		void AnimNotify_NOT_Teleport();
};
