// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemStruct.generated.h"

UENUM(BlueprintType)
enum class EEquipmentSlot : uint8
{
	None UMETA(DisplayName = "None"),
	Weapon UMETA(DisplayName = "Weapon"),
	Armor UMETA(DisplayName = "Armor"),
	Ring UMETA(DisplayName = "Ring"),
	// 추가 가능
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	None,
	Weapon,
	Armor,
	Ring,
	Consumable,
	KeyItem,
	Material,
	Quest,
};

UCLASS()
class UE5PROJECT_API AItemStruct : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemStruct();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};

