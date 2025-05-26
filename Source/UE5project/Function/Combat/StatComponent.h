// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../PEnumHeader.h"
#include "StatComponent.generated.h"

DECLARE_DELEGATE(FOnDeathDelegate);

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
	FOnDeathDelegate OnDeath;

public:
	void InitializeStats();

	void ChangeMaxHealth(float Amount);
	void ChangeMaxStamina(float Amount);
	void ChangeHealth(float Amount);
	void ChangeStamina(float Amount);


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float MaxHealth = 100.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float MaxStamina = 100.0f;
	float Defense;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float Health;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float Stamina;
	
	bool IsDead;
};