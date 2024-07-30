// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PEnumHeader.generated.h"

UENUM(BlueprintType)
enum class MovementMode : uint8
{
	Idle UMETA(DisplayName="Idle"),
	Walking UMETA(DisplayName = "Walking"),
	Jogging UMETA(DisplayName = "Jogging"),
	Sprinting UMETA(DisplayName = "Sprinting"),
};

UENUM(BlueprintType)
enum class MovementDirection : uint8
{
	Fwd UMETA(DisplayName = "FWD"),
	Bwd UMETA(DisplayName = "BWD"),
	Left UMETA(DisplayName = "LFT"),
	Right UMETA(DisplayName = "RGT"),
	FwdRight UMETA(DisplayName = "FRGT"),
	FwdLeft UMETA(DisplayName = "FLFT"),
	BwdRight UMETA(DisplayName = "BRGT"),
	BwdLeft UMETA(DisplayName = "BLFT"),
};

UENUM(BlueprintType)
enum class WarriorStance : uint8
{
	UA UMETA(DisplayName = "UA"),
	LS UMETA(DisplayName = "LS"),
	SNS UMETA(DisplayName = "SNS"),
};

UENUM(BlueprintType)
enum class HorseMovement : uint8
{
	Walk UMETA(DisplayName = "Walk"),
	Trot UMETA(DisplayName = "Trot"),
	Canter UMETA(DisplayName = "Canter"),
	Gallop UMETA(DisplayName = "Gallop"),
	Sprint UMETA(DisplayName = "Sprint"),
};

UENUM(BlueprintType)
enum class MontageType : uint8
{
	None UMETA(DisplayName = "None"),
	Attack UMETA(DisplayName = "Attack"),
	Swing1 UMETA(DisplayName = "Swing1"),
	Swing2 UMETA(DisplayName = "Swing2"),
	Swing3 UMETA(DisplayName = "Swing3"),
	Swing4 UMETA(DisplayName = "Swing4"),
	Appear UMETA(DisplayName = "Appear"),
	Dodge UMETA(DisplayName = "Dodge"),
	Swoop UMETA(DisplayName = "Swoop"),
	Block UMETA(DisplayName = "Block"),
	Death UMETA(DisplayName = "Death"),
	Hit UMETA(DisplayName = "Hit"),
	Parrying UMETA(DisplayName = "Parrying"),
	KnockBack UMETA(DisplayName = "KnockBack"),
	Collpase UMETA(DisplayName = "Collpase"),
	Fall UMETA(DisplayName = "Fall"),
};

UENUM(BlueprintType)
enum class HereticMontage : uint8
{
	None UMETA(DisplayName = "None"),
	Attack UMETA(DisplayName = "Attack"),
	Summon UMETA(DisplayName = "Summon"),
	Death UMETA(DisplayName = "Death"),
	Teleport UMETA(DisplayName = "Teleport"),
};

UENUM(BlueprintType)
enum class HereticSkill : uint8
{
	DarkBall UMETA(DisplayName = "DarkBall"),
	SawTooth UMETA(DisplayName = "SawTooth"),
	DarkSpear UMETA(DisplayName = "DarkSpear"),
	DarkBeam UMETA(DisplayName = "DarkBeam"),
	Tornado UMETA(DisplayName = "Tornado"),
	None UMETA(DisplayName = "None"),
};

UENUM(BlueprintType)
enum class WarriorVar : uint8
{
	Attack UMETA(DisplayName = "Attack"),
	HeavyAttack UMETA(DisplayName = "HeavyAttack"),
	Dodge UMETA(DisplayName = "Dodge"),
	Block UMETA(DisplayName = "Block"),
	Combo UMETA(DisplayName = "Combo"),
};

UENUM(BlueprintType)
enum class HereticVar : uint8
{
	Summon UMETA(DisplayName = "Summon"),
	Action UMETA(DisplayName = "Action"),
	DarkBall UMETA(DisplayName = "DarkBall"),
	SawTooth UMETA(DisplayName = "SawTooth"),
	DarkSpear UMETA(DisplayName = "DarkSpear"),
	DarkBeam UMETA(DisplayName = "DarkBeam"),
	Tornado UMETA(DisplayName = "Tornado"),
};

UENUM(BlueprintType)
enum class HSoldierVar : uint8
{
	IsAttack UMETA(DisplayName = "IsAttack"),
	IsHit UMETA(DisplayName = "IsHit"),
	IsHitLarge UMETA(DisplayName = "IsHitLarge"),
	IsBlock UMETA(DisplayName = "IsBlock"),
	IsDefenseMode UMETA(DisplayName = "IsDefenseMode"),
	IsParried UMETA(DisplayName = "IsParried"),
	IsBlockBreak UMETA(DisplayName = "IsBlockBreak"),
};

UENUM(BlueprintType)
enum class HSoldierMode : uint8
{
	OffenseMode UMETA(DisplayName = "OffenseMode"),
	DefenseMode UMETA(DisplayName = "DefenseMode"),
};

UENUM(BlueprintType)
enum class RangeCheck : uint8
{
	Far UMETA(DisplayName = "Far"),
	Proper UMETA(DisplayName = "Proper"),
	Close UMETA(DisplayName = "Close"),
};

UENUM(BlueprintType)
enum class RideVar : uint8
{
	Horizontal UMETA(DisplayName = "Horizontal"),
	Vertical UMETA(DisplayName = "Vertical"),
	IsBreak UMETA(DisplayName = "IsBreak"),
};

USTRUCT(BlueprintType)
struct FCharacterInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString CName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 CMaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 COffensePower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 CDefenseCap;

public:
	void SetName(FString Name)
	{
		CName = Name;
	}

	void SetMaxHP(int32 MaxHP)
	{
		CMaxHP = MaxHP;
	}

	void SetOffensePower(int32 OffensePower)
	{
		COffensePower = OffensePower;
	}

	void SetDefenseCap(int32 DefenseCap)
	{
		CDefenseCap = DefenseCap;
	}
};

USTRUCT(BlueprintType)
struct FEnemyInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString EName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 EMaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 EOffensePower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 EDefenseCap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 EDownPercent = 0;

public:
	void SetName(FString Name)
	{
		EName = Name;
	}

	void SetMaxHP(int32 MaxHP)
	{
		EMaxHP = MaxHP;
	}

	void SetOffensePower(int32 OffensePower)
	{
		EOffensePower = OffensePower;
	}

	void SetDefenseCap(int32 DefenseCap)
	{
		EDefenseCap = DefenseCap;
	}

	void SetDownPercent(int32 DownPercent)
	{
		EDownPercent = DownPercent;
	}
};

USTRUCT(BlueprintType)
struct FRangeInfo
{
	GENERATED_BODY()

public:
	float Melee;
	float Ranged;
	float Defend;
};

USTRUCT(BlueprintType)
struct FWeaponInfo
{
	GENERATED_BODY()

public:
	float AttackRange;
	float StrikingPower;
};

USTRUCT(BlueprintType)
struct FComponentTransform
{
	GENERATED_BODY()

public:
	FVector Location;
	FRotator Rotation;
};

UCLASS()
class UE5PROJECT_API APEnumHeader : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APEnumHeader();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		MovementMode CurMovementMode;
};
