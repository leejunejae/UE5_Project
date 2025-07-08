// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/StatComponent.h"

// Sets default values for this component's properties
UStatComponent::UStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UStatComponent::InitializeStats()
{
	Health = MaxHealth;
	Stamina = MaxStamina;
}

void UStatComponent::ChangeMaxHealth(const float Amount)
{
	MaxHealth += Amount;
}

void UStatComponent::ChangeMaxStamina(const float Amount)
{
	MaxStamina += Amount;
}

bool UStatComponent::ChangeHealth(const float Amount, const EHPChangeType HPChangeType)
{
	float Delta = Amount;
	switch (HPChangeType)
	{
	case EHPChangeType::DirectDamage:
		Delta *= (1.0f - (DefensivePower / (DefensivePower + 100.0f)));
		break;
	case EHPChangeType::TrueDamage:
		break;
	case EHPChangeType::Heal:
		Delta *= -1.0f;
		break;
	}

	Health = FMath::Clamp(Health - Delta, 0.0f, MaxHealth);
	
	if (Health <= 0.0f)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Death Delegate"));
		OnDeath.ExecuteIfBound();
	}

	return Health > 0.0f ? true : false;
}

bool UStatComponent::ChangeStamina(const float Amount, const ESPChangeType SPChangeType)
{
	float Delta = Amount;
	
	switch (SPChangeType)
	{
	case ESPChangeType::Blocked:
		Delta *= (1.0f - (GuardRate / 100.0f));
		break;
	case ESPChangeType::Dodge:
		break;
	case ESPChangeType::Heal:
		Delta *= -1.0f;
		break;
	}

	Stamina = FMath::Clamp(Stamina - Delta, 0.0f, MaxStamina);

	return Stamina > 0.0f ? true : false;
}