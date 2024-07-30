// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyHeader.generated.h"

UENUM(BlueprintType)
enum class HSoldierSkill : uint8
{
	None UMETA(DisplayName = "None"),
	Combo1 UMETA(DisplayName = "Combo1"),
	Combo2 UMETA(DisplayName = "Combo2"),
	Slash UMETA(DisplayName = "Summon"),
	Rush UMETA(DisplayName = "Death"),
};

UCLASS()
class UE5PROJECT_API AEnemyHeader : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyHeader();

};
