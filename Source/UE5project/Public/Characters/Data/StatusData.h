// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StatusData.generated.h"

UENUM(BlueprintType)
enum class ECharacterGroundState : uint8
{
	Normal UMETA(DisplayName = "Normal"),
	Attack UMETA(DisplayName = "Attack"),
	Block UMETA(DisplayName = "Block"),
	Dodge UMETA(DisplayName = "Dodge"),
	Jump UMETA(DisplayName = "Jump"),
	Hit UMETA(DisplayName = "Hit"),
	Invincible UMETA(DisplayName = "Invincible"),
	Parry UMETA(DislplayName = "Parry"),
	SuperArmor UMETA(DisplayName = "SuperArmor"),
};

UCLASS()
class UE5PROJECT_API AStatusData : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStatusData();
};
