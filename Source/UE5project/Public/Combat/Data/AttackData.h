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

USTRUCT(BlueprintType)
struct FAttackFeature
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName FeatureID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Amount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName SectionName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EHitResponse Response;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool CanBlocked;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool CanParried;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool CanAvoid;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<EStatusEffect> StatusEffectsList;

	inline bool operator==(const FAttackFeature& Other) const
	{
		return FeatureID == Other.FeatureID;
	}
};

uint32 GetTypeHash(const FAttackFeature& AttackFeature);

USTRUCT(Atomic, BlueprintType)
struct FNextAttackList : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName NextID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName Condition;

	inline bool operator==(const FNextAttackList& Other) const
	{
		return Condition == Other.Condition;
	}
};

uint32 GetTypeHash(const FNextAttackList& NextAttackList);

USTRUCT(Atomic, BlueprintType)
struct FAttackInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* Anim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "MontageVariable != nullptr"))
		FName SectionName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "MontageVariable != nullptr"))
		bool IsJumpAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "MontageVariable != nullptr"))
		TSet<FAttackFeature> Feature;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "MontageVariable != nullptr"))
		FText Discrption;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSet<FNextAttackList> NextIDList;

	inline bool operator==(const FAttackInfo& Other) const
	{
		return SectionName == Other.SectionName;
	}
};

uint32 GetTypeHash(const FAttackInfo& AttackInfo);

USTRUCT(Atomic, BlueprintType)
struct FAttackInfoList : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName AttackID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSet<FAttackInfo> AttackInfo;
};

UCLASS()
class UE5PROJECT_API UAttackData : public UObject
{
	GENERATED_BODY()
	
};
