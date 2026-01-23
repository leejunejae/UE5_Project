// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemies/Human/PBEHAnimInstance.h"
#include "Components/TimeLineComponent.h"
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
};
