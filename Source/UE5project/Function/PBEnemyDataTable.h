// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "PBEnemyDataTable.generated.h"

USTRUCT(BlueprintType)
struct FEnemyData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EMaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EDefaultDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EDefenseCap;
};

UCLASS()
class UE5PROJECT_API APBEnemyDataTable : public AActor
{
	GENERATED_BODY()
};
