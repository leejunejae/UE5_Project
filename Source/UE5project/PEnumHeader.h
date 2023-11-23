// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PEnumHeader.generated.h"

UENUM(BlueprintType)
enum class HitCheck : uint8
{
	None UMETA(DisplayName = "None"),
	Blocked UMETA(DisplayName = "Blocked"),
	Damaged UMETA(DisplayName = "Damaged"),
	Invincibled UMETA(DisplayName = "Invincibled"),
};

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
	Fwd,
	Bwd,
	Left,
	Right,
};

UENUM(BlueprintType)
enum class AttackType : uint8
{
	Sword UMETA(DisplayName = "Sword"),
	GreatSword UMETA(DisplayName = "GreatSword"),
	Arrow UMETA(DisplayName = "Arrow"),
	Axe UMETA(DisplayName = "Axe"),
	Hatchet UMETA(DisplayName = "Hatchet"),
	Magic UMETA(DisplayName = "Magic"),
	Environment UMETA(DisplayName = "Environment"),
	Punch UMETA(DisplayName = "Punch"),
	Kick UMETA(DisplayName = "Kick"),
};

UENUM(BlueprintType)
enum class HitResponse : uint8
{
	None UMETA(DisplayName = "None"),
	HitReaction UMETA(DisplayName = "HitReaction"),
	Stagger UMETA(DisplayName = "Stagger"),
	Stun UMETA(DisplayName = "Stun"),
	KnockBack UMETA(DisplayName = "KnockBack"),
	Fall UMETA(DisplayName = "Fall"),
	Collpase UMETA(DisplayName = "Collpase"),
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
	None UMETA(DisplayName = "None"),
	SummonSoldier UMETA(DisplayName = "SummonSoldier"),
	Darkball UMETA(DisplayName = "Darkball"),
};

UENUM(BlueprintType)
enum class HereticVar : uint8
{
	Summon UMETA(DisplayName = "Summon"),
	Action UMETA(DisplayName = "Action"),
};

UENUM(BlueprintType)
enum class RangeCheck : uint8
{
	Far UMETA(DisplayName = "Far"),
	Proper UMETA(DisplayName = "Proper"),
	Close UMETA(DisplayName = "Close"),
};

USTRUCT(BlueprintType)
struct FDamageInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Amount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AttackType Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		HitResponse Response;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Invincible;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool CanBlocked;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool CanParried;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool ForceInterrupt;
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
	AttackType WeaponType;
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
