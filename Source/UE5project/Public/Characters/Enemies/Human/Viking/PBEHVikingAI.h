// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemies/Human/PBEHumanAI.h"
#include "PBEHVikingAI.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API APBEHVikingAI : public APBEHumanAI
{
	GENERATED_BODY()
	
public:
	APBEHVikingAI();

	virtual void OnPossess(APawn* InPawn) override;
};
