// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "CharacterStatData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FStat_Vitality : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		int32 Level;

	UPROPERTY(EditAnywhere)
		float HP = 0.f;
};

USTRUCT(BlueprintType)
struct FStat_Endurance : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		int32 Level;

	UPROPERTY(EditAnywhere)
		float Stamina = 0.f;
};

USTRUCT(BlueprintType)
struct FStat_Mentality : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		int32 Level;

	UPROPERTY(EditAnywhere)
		float FP = 0.f;
};

USTRUCT(BlueprintType)
struct FStat_Strength : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		int32 Level;

	UPROPERTY(EditAnywhere)
		float MeleeDefense = 0.f;
};

USTRUCT(BlueprintType)
struct FStat_Dexterity : public FTableRowBase
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere)
		int32 Level;

	UPROPERTY(EditAnywhere)
		float RangedDefense = 0.f;
};

USTRUCT(BlueprintType)
struct FStat_Intelligence : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		int32 Level;

	UPROPERTY(EditAnywhere)
		float MagicDefense = 0.f;
};

USTRUCT(BlueprintType)
struct FStat_Vigor : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		int32 Level;

	UPROPERTY(EditAnywhere)
		float Poise = 0.f;

	UPROPERTY(EditAnywhere)
		float Resistance = 0.f;
};

USTRUCT(BlueprintType)
struct FCharacterStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Vitality = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Endurance = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Mentality = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Strength = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Dexterity = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Intelligence = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Vigor = 10;

	// 스탯 합산, 비교, 증감 같은 유틸 함수도 여기 포함 가능
	FCharacterStats operator+(const FCharacterStats& Other) const
	{
		return {
			Vitality + Other.Vitality,
			Endurance + Other.Endurance,
			Mentality + Other.Mentality,
			Strength + Other.Strength,
			Dexterity + Other.Dexterity,
			Intelligence + Other.Intelligence,
			Vigor + Other.Vigor
		};
	}

	float GetRequirementStatRate(const FCharacterStats& Requirement) const;
};

UCLASS()
class UE5PROJECT_API UCharacterStatData : public UObject
{
	GENERATED_BODY()
	
};
