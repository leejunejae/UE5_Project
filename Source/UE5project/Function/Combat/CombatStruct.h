// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "CombatStruct.generated.h"


// 피격 유형 결정
UENUM(BlueprintType)
enum class HitResponse : uint8
{
	None UMETA(DisplayName = "None"),
	Block UMETA(DisplayName = "Block"),
	BlockLarge UMETA(DisplayName = "BlockLarge"),
	BlockBreak UMETA(DisplayName = "BlockBreak"),
	Parry UMETA(DisplayName = "Parry"),
	Hit UMETA(DisplayName = "Hit"),
	HitLarge UMETA(DisplayName = "HitLarge"),
	HitAir UMETA(DisplayName = "HitAir"),
	Invincible UMETA(DisplayName = "Invincible"),
	Fatal UMETA(DisplayName = "Fatal"),
	Dead UMETA(DislplayName = "Dead"),
	KnockDown UMETA(DislplayName = "KnockDown"),
};

USTRUCT(Atomic, BlueprintType)
struct FHitData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* Anim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "MontageVariable != nullptr"))
		FName SectionName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "MontageVariable != nullptr"))
		FText Discrption;

	inline bool operator==(const FHitData& Other) const
	{
		return SectionName == Other.SectionName;
	}
};

/*
* 공격 데이터 자료형
*/
UENUM(BlueprintType)
enum class Debuff : uint8
{
	Bleeding UMETA(DisplayName = "Bleeding"),
	Darkness UMETA(DisplayName = "Darkness"),
	SlowDown UMETA(DisplayName = "SlowDown"),
	Weakness UMETA(DisplayName = "Weakness"),
	Restraint UMETA(DisplayName = "Restraint"),
};

UENUM(BlueprintType)
enum class AttackType : uint8
{
	Light UMETA(DisplayName = "Light"),
	Heavy UMETA(DisplayName = "Heavy"),
	AOE UMETA(DisplayName = "AOE"),
};


USTRUCT(BlueprintType)
struct FAttackInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName FeatureID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool NonDamaging;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "!NonDamaging"))
		float Amount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "!NonDamaging"))
		AttackType Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<Debuff> DebuffList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool CanBlocked;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool CanParried;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool CanAvoid;

	inline bool operator==(const FAttackInfo& Other) const
	{
		return FeatureID == Other.FeatureID;
	}
};

uint32 GetTypeHash(const FAttackInfo& AttackInfo);

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
struct FAttackData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* Anim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "MontageVariable != nullptr"))
		FName SectionName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "MontageVariable != nullptr"))
		bool IsLoop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "IsLoop"))
		int32 LoopNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "IsLoop"))
		float LoopTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "MontageVariable != nullptr"))
		bool IsJumpAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "MontageVariable != nullptr"))
		bool NeedMotionWarp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "MontageVariable != nullptr"))
		TSet<FAttackInfo> Feature;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "MontageVariable != nullptr"))
		FText Discrption;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSet<FNextAttackList> NextIDList;

	inline bool operator==(const FAttackData& Other) const
	{
		return SectionName == Other.SectionName;
	}
};

uint32 GetTypeHash(const FAttackData& AttackData);

USTRUCT(Atomic, BlueprintType)
struct FAttackDataStruct : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSet<FAttackData> AttackDataList;
};

/*
* 피격 대상 감지를 위한 자료형
*/
UENUM(BlueprintType)
enum class TraceType : uint8
{
	Line UMETA(DisplayName = "Line"),
	Sphere UMETA(DisplayName = "Sphere"),
	Capsule UMETA(DisplayName = "Capsule"),
	Box UMETA(DisplayName = "Box"),
};

USTRUCT(BlueprintType)
struct FAttackRange
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector RangeBegin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector RangeEnd;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector AddValue;

	FAttackRange() :AddValue(FVector::ZeroVector) {};
};

UCLASS()
class UE5PROJECT_API ACombatStruct : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACombatStruct();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
