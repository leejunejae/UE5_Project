// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ClimbHeader.generated.h"


USTRUCT(BlueprintType)
struct FGrabData
{
	GENERATED_BODY()

public:
	FName MainBone;
	FName SubBone;
	FName BoneCurveName;
	float AdjustMultiplier;
	FVector TargetLocation;

public:
	FGrabData() {}
	FGrabData(
	FName InMainBone,
	FName InSubBone,
	FName InBoneCurveName,
	float InAdjustMultiplier,
	FVector InTargetLocation
	)
	: MainBone(InMainBone)
	, SubBone(InSubBone)
	, BoneCurveName(InBoneCurveName)
	, AdjustMultiplier(InAdjustMultiplier)
	, TargetLocation(InTargetLocation)
	{}

	inline bool operator==(const FGrabData& Other) const
	{
		return MainBone == Other.MainBone;
	}
};

uint32 GetTypeHash(const FGrabData& GrabData);

USTRUCT(Atomic, BlueprintType)
struct FNeighborInfo
{
	GENERATED_BODY()

	FGripNode1D* Neighbor;
	float Distance;

	inline bool operator==(const FNeighborInfo& Other) const
	{
		return Neighbor == Other.Neighbor;
	}
};

USTRUCT(BlueprintType)
struct FGripNode2D
{
	GENERATED_BODY()

	FVector Position;
	TArray<FName> Tag;
	TArray<FGripNode2D*> NeighborsUp;
	TArray<FGripNode2D*> NeighborsDown;
	TArray<FGripNode2D*> NeighborsRight;
	TArray<FGripNode2D*> NeighborsLeft;
};

USTRUCT(Atomic,BlueprintType)
struct FGripNode1D
{
	GENERATED_BODY()

	FVector Position;
	int32 Level;
	int32 GripIndex;
	TArray<FName> Tag;
	FNeighborInfo NeighborUp;
	FNeighborInfo NeighborDown;
	FGripNode1D* PrevGrip;

	inline bool operator==(const FGripNode1D& Other) const
	{
		return Position == Other.Position;
	}
};

UCLASS()
class UE5PROJECT_API UClimbHeader : public UObject
{
	GENERATED_BODY()

};
