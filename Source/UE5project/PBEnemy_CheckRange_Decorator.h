// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "PBEnemy_CheckRange_Decorator.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API UPBEnemy_CheckRange_Decorator : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UPBEnemy_CheckRange_Decorator();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Range", meta = (ExposeOnSpawn = "true", OverrideNativeName = "bOnHand"))
		FBlackboardKeySelector IdealRange;
};
