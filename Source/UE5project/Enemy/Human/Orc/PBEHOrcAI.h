// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../PBEHumanAI.h"
#include "PBEHOrcAI.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API APBEHOrcAI : public APBEHumanAI
{
	GENERATED_BODY()

public:
	APBEHOrcAI();

	virtual void OnPossess(APawn* InPawn) override;

	virtual void StopAI();
};
