// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "PBEKraken_FindTarget_Service.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API UPBEKraken_FindTarget_Service : public UBTService
{
	GENERATED_BODY()

public:
	UPBEKraken_FindTarget_Service();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
