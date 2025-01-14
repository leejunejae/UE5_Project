// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ClimbHeader.generated.h"

USTRUCT(BlueprintType)
struct FNeighborInfo
{
	GENERATED_BODY()

	FGripNode1D* Neighbor;
	float Distance;
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

USTRUCT(BlueprintType)
struct FGripNode1D
{
	GENERATED_BODY()

	FVector Position;
	int32 Level;
	int32 GripIndex;
	TArray<FName> Tag;
	FNeighborInfo NeighborUp;
	FNeighborInfo NeighborDown;
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
