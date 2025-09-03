// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Characters/Data/CharacterStatData.h"
#include "Characters/Interfaces/StatInterface.h"
#include "Characters/Interfaces/DeathInterface.h"
#include "Combat/Data/CombatData.h"
#include "PEnumHeader.h"
#include "StatComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5PROJECT_API UStatComponent : public UActorComponent,
	public IStatInterface,
	public IDeathInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	FOnDeathDelegate OnDeath;
	virtual FOnDeathDelegate& GetOnDeathDelegate() override { return OnDeath; }

public:
	void InitializeStats();

	void ChangeMaxHealth(const float Amount);
	void ChangeMaxStamina(const float Amount);
	void ChangeMaxPoise(const float Amount);
	bool ApplyDamage(const float Amount, const EAttackType AttackType);
	bool Heal(const float Amount);
	bool ChangeStamina(const float Amount, const EStatChangeType SPChangeType);
	bool ChangePoise(const float Amount, const EStatChangeType PoiseChangeType);

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