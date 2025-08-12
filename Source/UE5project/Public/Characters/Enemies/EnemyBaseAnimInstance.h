// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyBaseAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API UEnemyBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UEnemyBaseAnimInstance();
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
