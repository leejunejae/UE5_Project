// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CombatData.h"
#include "GameFramework/Actor.h"
#include "HitReactionData.generated.h"

UENUM(BlueprintType)
enum class EAnimLoopType : uint8
{
	Time UMETA(DisplayName="Time"),
	Condition UMETA(DisplayName="Condition")
};

UENUM(BlueprintType)
enum class EHitPointVertical : uint8
{
	Neutral UMETA(DisplayName = "Neutral"),
	Up UMETA(DisplayName = "Up"),
	Down UMETA(DisplayName = "Down"),
};

UENUM(BlueprintType)
enum class EHitPointHorizontal : uint8
{
	Neutral UMETA(DisplayName = "Neutral"),
	Left UMETA(DisplayName = "Left"),
	Right UMETA(DisplayName = "Right"),
	Back UMETA(DisplayName = "Back"),
	Front UMETA(DisplayName = "Front"),
	FrontRight UMETA(DisplayName = "FrontRight"),
	FrontLeft UMETA(DisplayName = "FrontLeft"),
	BackRight UMETA(DisplayName = "BackRight"),
	BackLeft UMETA(DisplayName = "BackLeft"),
};

USTRUCT(Atomic, BlueprintType)
struct FHitReactionRequest
{
	GENERATED_BODY()

public:
	HitResponse Response;
	bool CanBlocked;
	bool CanParried;
	bool CanAvoid;
	FVector HitPoint;
};

USTRUCT(Atomic, BlueprintType)
struct FHitReactionInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* Anim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "Anim != nullptr"))
		FName SectionName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "Anim != nullptr"))
		EHitPointVertical HitPointVertical;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "Anim != nullptr"))
		EHitPointHorizontal HitPointHorizontal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "Anim != nullptr"))
		bool IsLoop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "IsLoop"))
		EAnimLoopType AnimLoopType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "IsLoop && AnimLoopType == EAnimLoopType::Time"))
		float LoopTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "Anim != nullptr"))
		FText Discription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "Anim != nullptr"))
		bool HasNextReaction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "HasNextReaction == true"))
		FName NextSection;

	inline bool operator==(const FHitReactionInfo& Other) const
	{
		return SectionName == Other.SectionName;
	}
};

uint32 GetTypeHash(const FHitReactionInfo& HitReactionInfo);

USTRUCT(Atomic, BlueprintType)
struct FHitReactionInfoList : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		HitResponse HitReactionID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSet<FHitReactionInfo> HitReactionInfo;
};

UCLASS()
class UE5PROJECT_API AHitReactionData : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHitReactionData();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
