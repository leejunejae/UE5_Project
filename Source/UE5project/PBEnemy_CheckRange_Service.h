// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "PBEnemy_CheckRange_Service.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API UPBEnemy_CheckRange_Service : public UBTService
{
	GENERATED_BODY()
	
public:
	UPBEnemy_CheckRange_Service();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Range", meta = (ExposeOnSpawn = "true", OverrideNativeName = "bOnHand"))
		FBlackboardKeySelector IdealRange;
};
