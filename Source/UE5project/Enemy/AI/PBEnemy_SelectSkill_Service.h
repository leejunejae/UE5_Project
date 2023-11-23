// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../PEnumHeader.h"
#include "BehaviorTree/BTService.h"
#include "PBEnemy_SelectSkill_Service.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API UPBEnemy_SelectSkill_Service : public UBTService
{
	GENERATED_BODY()
	
public:
	UPBEnemy_SelectSkill_Service();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
