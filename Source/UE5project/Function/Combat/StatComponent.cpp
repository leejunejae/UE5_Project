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

void UStatComponent::ChangeMaxHealth(float Amount)
{
	MaxHealth += Amount;
}

void UStatComponent::ChangeMaxStamina(float Amount)
{
	MaxStamina += Amount;
}

void UStatComponent::ChangeHealth(float Amount)
{
	Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);

	if (Health <= 0.0f)
	{
		IsDead = true;
	}
}

void UStatComponent::ChangeStamina(float Amount)
{
	Stamina = FMath::Clamp(Stamina + Amount, 0.0f, MaxStamina);
}