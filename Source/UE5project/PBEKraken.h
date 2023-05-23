// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PBEnemy.h"
#include "PBEKraken.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API APBEKraken : public APBEnemy
{
	GENERATED_BODY()

public:
	APBEKraken();

protected:
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
