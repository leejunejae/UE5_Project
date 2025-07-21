// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Combat/Data/AttackData.h"
#include "Items/Weapons/Data/WeaponData.h"
#include "PlayerAttackDataAsset.generated.h"

/**
 * 
 */


USTRUCT(Atomic, BlueprintType)
struct FPlayerAttackDetail
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FBaseAttackData BaseAttackData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DamageMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float PoiseDamageMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float StanceDamageMultiplier;
	/*
	inline bool operator==(const FPlayerAttackDetail& Other) const
	{
		return SectionName == Other.SectionName;
	}
	*/
};

//uint32 GetTypeHash(const FPlayerAttackDetail& AttackData);

USTRUCT(Atomic, BlueprintType)
struct FPlayerAttackInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName AttackName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* Anim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FPlayerAttackDetail> AttackDetail;

	inline bool operator==(const FPlayerAttackInfo& Other) const
	{
		return AttackName == Other.AttackName;
	}
};

uint32 GetTypeHash(const FPlayerAttackInfo& AttackType);

USTRUCT(Atomic, BlueprintType)
struct FPlayerAttackInfoList
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSet<FPlayerAttackInfo> AttackInfoList;
};

UCLASS()
class UE5PROJECT_API UPlayerAttackDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<EWeaponType, FPlayerAttackInfoList> AttackInfoList;

	FPlayerAttackInfoList FindPlayerAttackInfo(const EWeaponType& WeaponType, bool bLogNotFound = false) const;
};
