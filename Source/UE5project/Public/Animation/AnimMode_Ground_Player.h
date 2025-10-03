// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimMode_Ground.h"
#include "AnimMode_Ground_Player.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API UAnimMode_Ground_Player : public UAnimMode_Ground
{
	GENERATED_BODY()
	
public:
	virtual void Tick(float DeltaSeconds) override;
};
