// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "PBEnemy_CheckAttack_Decorator.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API UPBEnemy_CheckAttack_Decorator : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UPBEnemy_CheckAttack_Decorator();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
