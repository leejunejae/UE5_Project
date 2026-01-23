// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemies/EnemyBase.h"
#include "PBEHuman.generated.h"

/**
 * 
 */

UCLASS()
class UE5PROJECT_API APBEHuman : public AEnemyBase
{
	GENERATED_BODY()

public:
	APBEHuman(const FObjectInitializer& ObjectInitializer);

};