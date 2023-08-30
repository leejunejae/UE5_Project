// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlueprintBase.h"
#include "PBEnemy_CheckDistance_Decorator.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API UPBEnemy_CheckDistance_Decorator : public UBTDecorator_BlueprintBase
{
	GENERATED_BODY()
	
public:
	UPBEnemy_CheckDistance_Decorator();
};