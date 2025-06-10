// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../PEnumHeader.h"
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
class UE5PROJECT_API UStatComponent : public UActorComponent
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
	FORCEINLINE float GetDefensePower() const { return DefensivePower; }


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float MaxHealth = 100.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float MaxStamina = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float Health;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float Stamina;


	//float StrikingPower;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float DefensivePower;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float GuardRate;
};