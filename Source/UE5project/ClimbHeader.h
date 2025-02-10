// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ClimbHeader.generated.h"

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
class UE5PROJECT_API AClimbHeader : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AClimbHeader();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
