// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PBCharacter.h"
#include "PCWarrior.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API APCWarrior : public APBCharacter
{
	GENERATED_BODY()
	
public:
	APCWarrior();

protected:
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void MoveSpeedToggle() override;

private:
	FTimerHandle MoveSpeedTimerHandle;
	void MoveSpeedTimer();
};