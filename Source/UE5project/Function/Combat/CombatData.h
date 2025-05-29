// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombatData.generated.h"


// 피격 유형 결정
UENUM(BlueprintType)
enum class HitResponse : uint8
{
	None UMETA(DisplayName = "None"),
	Flinch UMETA(DisplayName = "Flinch"),
	KnockBack UMETA(DisplayName = "KnockBack"),
	KnockDown UMETA(DislplayName = "KnockDown"),
	HitAir UMETA(DislplayName = "HitAir"),
	Block UMETA(DisplayName = "Block"),
	BlockLarge UMETA(DisplayName = "BlockLarge"),
	BlockBreak UMETA(DisplayName = "BlockBreak"),
	BlockStun UMETA(DisplayName = "BlockStun"),
	Parry UMETA(DisplayName = "Parry"),
	Death UMETA(DislplayName = "Death"),
	DeathLarge UMETA(DislplayName = "DeathLarge"),
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

/*
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
*/

UCLASS()
class UE5PROJECT_API ACombatData : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACombatData();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
