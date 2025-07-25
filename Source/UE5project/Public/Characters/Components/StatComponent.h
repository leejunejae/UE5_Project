// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Characters/Data/CharacterStatData.h"
#include "Characters/Interfaces/StatInterface.h"
#include "PEnumHeader.h"
#include "StatComponent.generated.h"

DECLARE_DELEGATE(FOnSingleDelegate);


UENUM()
enum class EHPChangeType : uint8
{
	DirectDamage UMETA(DisplayName = "DirectDamage"),
	TrueDamage UMETA(DisplayName = "TrueDamage"),
	Heal UMETA(DisplayName = "Heal"),
};

UENUM()
enum class ESPChangeType : uint8
{
	Blocked UMETA(DisplayName = "Blocked"),
	Dodge UMETA(DisplayName = "Dodge"),
	Heal UMETA(DisplayName = "Heal"),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5PROJECT_API UStatComponent : public UActorComponent,
	public IStatInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	FOnSingleDelegate OnDeath;

public:
	void InitializeStats();

	void ChangeMaxHealth(const float Amount);
	void ChangeMaxStamina(const float Amount);
	void ChangeMaxPoise(const float Amount);
	bool ChangeHealth(const float Amount, const EHPChangeType HPChangeType);
	bool ChangeStamina(const float Amount, const ESPChangeType SPChangeType);
	bool ChangePoise(const float Amount);

	FORCEINLINE FCharacterStats GetBaseStats_Native() const { return BaseStats; }

	FCharacterAttributes GetBaseAttributesLevel_Implementation() const { return BaseAttributes; }
	FCharacterStats GetBaseStats_Implementation() const { return BaseStats; }
	float GetAttributesRequirementRatio_Implementation(const FCharacterAttributes& RequireStats) const;
	float GetWeaponPerformanceRatio_Implementation(const FCharacterAttributes& RequireStats) const;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Stats")
		TMap<EAttributeType, UDataTable*> AttributeTables;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats") // 기본 특성값(힘, 민첩, 의지 등)
		FCharacterAttributes BaseAttributes; 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats") // 특성값과 장비를 비롯한 요소로 결정되는 수치화된 캐릭터의 능력
		FCharacterStats BaseStats;
};