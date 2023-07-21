// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../PBEnemy.h"
#include "PBEHuman.generated.h"

/**
 * 
 */
class UCharacterMovementComponent;

UCLASS()
class UE5PROJECT_API APBEHuman : public APBEnemy
{
	GENERATED_BODY()
	
public:
	APBEHuman();
};
