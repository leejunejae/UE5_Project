// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "Characters/Data/AffiliationData.h"
#include "Characters/Enemies/Data/EnemyComponentDataAsset.h"
#include "Characters/Enemies/Data/EnemyInstanceDataAsset.h"
#include "Characters/Enemies/Data/EnemyAIDataAsset.h"
#include "EnemyData.generated.h"

UENUM(BlueprintType)
enum class HSoldierSkill : uint8
{
	None UMETA(DisplayName = "None"),
	Combo1 UMETA(DisplayName = "Combo1"),
	Combo2 UMETA(DisplayName = "Combo2"),
	Slash UMETA(DisplayName = "Summon"),
	Rush UMETA(DisplayName = "Death"),
};


USTRUCT(BlueprintType)
struct FEnemyStats :public FTableRowBase
{
    GENERATED_BODY();

    /** 내부 식별용 ID */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
        FName ID;

    /** 적 이름 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
        FText Name;

    /** 적 위치 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
        FText Location;

    /** 체력 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
        float Health;

    /** 강인도(강인도가 0이 되면 무력화 상태) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
        float Poise;

    /** 스탠스(스탠스가 0이 되면 경직 반응) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
        float Stance;

    /** 물리 방어력 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
        float PhysicalDefense;

    /** 마법 방어력 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
        float MagicDefense;

    /** 데미지 감쇠 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
        float DamageNegation;

    /** 상태이상 저항 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
        float Resistance;
};

USTRUCT(BlueprintType)
struct FEnemyInfo : public FTableRowBase
{
    GENERATED_BODY();

    /** 내부 식별용 ID */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
        FName ID;

    /** 적 이름 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
        FText Name;

    /** 적 위치 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
        FText Location;

    /** 적 세력 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
        EFaction Faction;

    /** 적 종족 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
        ERace Race;

    /** 드랍 테이블 */
   // UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Refs")
       // TSoftObjectPtr<ULootTableDataAsset> LootTable;

    /** AI / 행동 세트 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
        TSoftObjectPtr<UEnemyAIDataAsset> AIData;

    /** 메시/애니/사운드 등 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
        TSoftObjectPtr<UEnemyInstanceDataAsset> InstanceData;

    /** 컴포넌트 데이터 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
        TSoftObjectPtr<UEnemyComponentDataAsset> ComponentData;
};

// 참조, 캐싱, 반환을 위한 데이터 묶음
USTRUCT(BlueprintType)
struct FEnemyDataSet
{
    GENERATED_BODY();

public:
    const FEnemyInfo* Info;
    const FEnemyStats* Stats;

public:
    FEnemyDataSet() {}

    FEnemyDataSet(const FEnemyInfo* NewInfo, const FEnemyStats* NewStats)
        : Info(NewInfo), Stats(NewStats)
    {}
};

USTRUCT(BlueprintType)
struct FEnemyRuntimeStats
{
    GENERATED_BODY();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
        float MaxHealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
        float Health;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
        float MaxPoise;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
        float Poise;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
        float MaxStance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
        float Stance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
        float PhysicalDefense;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
        float MagicDefense;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
        float DamageNegation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
        float Resistance;

    void InitFromDataTable(const FEnemyStats& Data)
    {
        MaxHealth = Data.Health;
        Health = MaxHealth;
        MaxPoise = Data.Poise;
        Poise = MaxPoise;
        MaxStance = Data.Stance;
        Stance = MaxStance;
        PhysicalDefense = Data.PhysicalDefense;
        MagicDefense = Data.MagicDefense;
        DamageNegation = Data.DamageNegation;
    }
};

UCLASS()
class UE5PROJECT_API UEnemyData : public UObject
{
	GENERATED_BODY()

};
