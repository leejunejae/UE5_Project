// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Items/Weapons/Data/WeaponDataAsset.h"
#include "WeaponData.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	None UMETA(DisplayName = "None"),
	SwordAndShield UMETA(DisplayName = "SwordAndShield"),
	LongSword UMETA(DisplayName = "LongSword"),
	GreatSword UMETA(DisplayName = "GreatSword"),
	SpearAndShield UMETA(DisplayName = "SpearAndShield"),
	Knuckles UMETA(DisplayName = "Knuckles"),
};

UENUM(BlueprintType)
enum class ETraceType : uint8
{
	None UMETA(DisplayName = "None"),
	Box UMETA(DisplayName = "Box"),
	Capsule UMETA(DisplayName = "Capsule"),
	GreatSword UMETA(DisplayName = "GreatSword"),
	SpearAndShield UMETA(DisplayName = "SpearAndShield"),
	Knuckles UMETA(DisplayName = "Knuckles"),
};

USTRUCT(BlueprintType)
struct FWeaponStatRequirement
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Strength = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Dexterity = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Intelligence = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Willpower = 0;

	// 유틸: 현재 능력치가 요구치를 충족하는지 비교
	bool WeaponStatIsSatisfied(const FWeaponStatRequirement& PlayerStat) const
	{
		return PlayerStat.Strength >= Strength &&
			PlayerStat.Dexterity >= Dexterity &&
			PlayerStat.Intelligence >= Intelligence &&
			PlayerStat.Willpower >= Willpower;
	}

	float WeaponStatSatisfactionRate(const FWeaponStatRequirement& PlayerStats) const
	{
		auto RatioFor = [](int32 Required, int32 Actual)
		{
			if (Required <= 0) return 1.f;
			return FMath::Clamp((float)Actual / (float)Required, 0.f, 1.f);
		};

		float StrengthRatio = RatioFor(Strength, PlayerStats.Strength);
		float DexterityRatio = RatioFor(Dexterity, PlayerStats.Dexterity);
		float IntelligenceRatio = RatioFor(Intelligence, PlayerStats.Intelligence);
		float WillpowerRatio = RatioFor(Willpower, PlayerStats.Willpower);

		float AvgRatio = (StrengthRatio + DexterityRatio + IntelligenceRatio + WillpowerRatio) / 4.0f;

		return FMath::Lerp(0.5f, 1.0f, AvgRatio);
	}
};


USTRUCT(Atomic, BlueprintType)
struct FWeaponPartInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UWeaponDataAsset> WeaponInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTransform WeaponTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float HitBoxRadius;
};

USTRUCT(Atomic, BlueprintType)
struct FWeaponSetsInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText WeaponName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FWeaponPartInfo MainWeapon;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		bool HasSubWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "HasSubWeapon"))
		FWeaponPartInfo SubWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AttackPower; // 공격력

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float PoisePower; // 경직치

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float StancePower; // 스탠스 데미지

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float PoiseBonus; // 공격 시 추가되는 경직 보너스

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float GuardNegation; // 가드시 경감률(가드시 데미지 감소율)

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float GuardBoost; // 가드 강도(값만큼 퍼센트로 들어온 스태미나 소모율 감소)

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FWeaponStatRequirement RequiredStats;

public:
	FWeaponSetsInfo(){}
};