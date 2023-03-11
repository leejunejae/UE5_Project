// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PCWAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API UPCWAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPCWAnimInstance();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
	float CurrentSpeed;
};
