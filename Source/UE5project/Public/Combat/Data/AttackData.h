// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/Data/CombatData.h"
#include "Items/Weapons/Data/WeaponData.h"
#include "Utils/AnimBoneTransformDataAsset.h"
#include "Engine/DataTable.h"
#include "Engine/DataAsset.h"
#include "UObject/NoExportTypes.h"
#include "AttackData.generated.h"

USTRUCT(BlueprintType)
struct FBaseAttackData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName SectionName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UAnimBoneTransformDataAsset> TargetBoneTransformDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EHitResponse Response;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EAttackType AttackType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool CanBlocked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool CanParried;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool CanAvoid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FStatusEffect> StatusEffectList;
};

UCLASS()
class UE5PROJECT_API UAttackData : public UObject
{
	GENERATED_BODY()
	
};
