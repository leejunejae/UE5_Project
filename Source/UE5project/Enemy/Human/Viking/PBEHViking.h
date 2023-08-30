// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../PBEHuman.h"
#include "PBEHViking.generated.h"

/**
 * 
 */
class UPBEHVAnimInstance;

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UCLASS()
class UE5PROJECT_API APBEHViking : public APBEHuman
{
	GENERATED_BODY()
	
public:
	APBEHViking();
	virtual void Tick(float DeltaTime) override;

	virtual void Attack() override;
public:
	FOnAttackEndDelegate OnAttackEnd;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = Animation)
		class UPBEHVAnimInstance* VikingAnim;

private:
	virtual void PostInitializeComponents() override;

	UFUNCTION()
		void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
};