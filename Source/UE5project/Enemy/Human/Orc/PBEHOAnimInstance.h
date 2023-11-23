// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../PBEHAnimInstance.h"
#include "PBEHOAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API UPBEHOAnimInstance : public UPBEHAnimInstance
{
	GENERATED_BODY()

public:
	UPBEHOAnimInstance();
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	//class APBEHViking* Character = nullptr;
};
