// Fill out your copyright notice in the Description page of Project Settings.


#include "StatComponent.h"

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

void UStatComponent::ChangeHealth(const float Amount, const EHPChangeType HPChangeType)
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

	UE_LOG(LogTemp, Warning, TEXT("%f"), Health);

	if (Health <= 0.0f)
	{
		IsDead = true;
	}
}

void UStatComponent::ChangeStamina(const float Amount, const ESPChangeType SPChangeType)
{
	Stamina = FMath::Clamp(Stamina - Amount, 0.0f, MaxStamina);
}