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
	bool ChangeHealth(const float Amount, const EHPChangeType HPChangeType);
	bool ChangeStamina(const float Amount, const ESPChangeType SPChangeType);

	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE float GetStamina() const { return Stamina; }
	FORCEINLINE float GetDefensePower() const { return MeleeDefense; }

	FCharacterStats GetBaseStatLevel_Implementation() const;
	float GetStatRequirementRatio_Implementation(const FCharacterStats& RequireStats) const;
	float GetWeaponPerformanceRatio_Implementation(const FCharacterStats& RequireStats) const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Stat Tables")
		UDataTable* VitalityTable;

	UPROPERTY(EditDefaultsOnly, Category = "Stat Tables")
		UDataTable* EnduranceTable;

	UPROPERTY(EditDefaultsOnly, Category = "Stat Tables")
		UDataTable* MentalityTable;

	UPROPERTY(EditDefaultsOnly, Category = "Stat Tables")
		UDataTable* StrengthTable;

	UPROPERTY(EditDefaultsOnly, Category = "Stat Tables")
		UDataTable* DexterityTable;

	UPROPERTY(EditDefaultsOnly, Category = "Stat Tables")
		UDataTable* IntelligenceTable;

	UPROPERTY(EditDefaultsOnly, Category = "Stat Tables")
		UDataTable* VigorTable;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
		FCharacterStats BaseStats;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float MaxHealth = 500.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float MaxStamina = 100.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float MaxFocus = 100.0f;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float Health;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float Stamina;


	//float StrikingPower;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float MeleeDefense;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float RangedDefense;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float MagicDefense;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
		float Poise;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
		float Resistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float GuardRate;
};