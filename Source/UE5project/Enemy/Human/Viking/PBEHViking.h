// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../PBEHuman.h"
#include "PBEHViking.generated.h"

/**
 * 
 */
class UPBEHVAnimInstance;


UCLASS()
class UE5PROJECT_API APBEHViking : public APBEHuman
{
	GENERATED_BODY()
	
public:
	APBEHViking();
	virtual void Tick(float DeltaTime) override;
	virtual void Attack(FName AttackName, ACharacter* Target = nullptr) override;
	virtual void Death() override;
	//virtual void Block(bool CanParried) override;

public:

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = Animation)
		class UPBEHVAnimInstance* VikingAnim;

private:
	virtual void PostInitializeComponents() override;

	void IsMontageEnded(UAnimMontage* Montage, bool bInterrupted) override;
};