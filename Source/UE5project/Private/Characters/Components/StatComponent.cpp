// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/StatComponent.h"
#include "..\..\..\Public\Characters\Components\StatComponent.h"

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

	InitializeStats();

	// ...
	
}

void UStatComponent::InitializeStats()
{
	if (VitalityTable)
	{
		FString RowName = FString::Printf(TEXT("Vitality_%d"), BaseStats.Vitality);
		const FStat_Vitality* VitalityDTRow = VitalityTable->FindRow<FStat_Vitality>(FName(*RowName), TEXT(""));
		
		if (VitalityDTRow)
		{
			MaxHealth = VitalityDTRow->HP;
			Health = MaxHealth;
		}
		else 
		{
			UE_LOG(LogTemp, Warning, TEXT("Vitality is Unloaded"));
		}
	}

	if (EnduranceTable)
	{
		FString RowName = FString::Printf(TEXT("Endurance_%d"), BaseStats.Endurance);
		const FStat_Endurance* EnduranceDTRow = EnduranceTable->FindRow<FStat_Endurance>(FName(*RowName), TEXT(""));

		if (EnduranceDTRow)
		{
			MaxStamina = EnduranceDTRow->Stamina;
			Stamina = MaxStamina;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Endurance is Unloaded"));
		}
	}

	if (MentalityTable)
	{
		FString RowName = FString::Printf(TEXT("Mentality_%d"), BaseStats.Mentality);
		const FStat_Mentality* MentalityDTRow = MentalityTable->FindRow<FStat_Mentality>(FName(*RowName), TEXT(""));

		if (MentalityDTRow)
		{
			MaxFocus = MentalityDTRow->FP;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Mentality is Unloaded"));
		}
	}

	if (StrengthTable)
	{
		FString RowName = FString::Printf(TEXT("Strength_%d"), BaseStats.Strength);
		const FStat_Strength* StrengthDTRow = StrengthTable->FindRow<FStat_Strength>(FName(*RowName), TEXT(""));

		if (StrengthDTRow)
		{
			MeleeDefense = StrengthDTRow->MeleeDefense;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Strength is Unloaded"));
		}
	}

	if (DexterityTable)
	{
		FString RowName = FString::Printf(TEXT("Dexterity_%d"), BaseStats.Dexterity);
		const FStat_Dexterity* DexterityDTRow = DexterityTable->FindRow<FStat_Dexterity>(FName(*RowName), TEXT(""));

		if (DexterityDTRow)
		{
			RangedDefense = DexterityDTRow->RangedDefense;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Dexterity is Unloaded"));
		}
	}

	if (IntelligenceTable)
	{
		FString RowName = FString::Printf(TEXT("Intelligence_%d"), BaseStats.Intelligence);
		const FStat_Intelligence* IntelligenceDTRow = IntelligenceTable->FindRow<FStat_Intelligence>(FName(*RowName), TEXT(""));

		if (IntelligenceDTRow)
		{
			MagicDefense = IntelligenceDTRow->MagicDefense;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Intelligence is Unloaded"));
		}
	}

	if (VigorTable)
	{
		FString RowName = FString::Printf(TEXT("Vigor_%d"), BaseStats.Vigor);
		const FStat_Vigor* VigorDTRow = VigorTable->FindRow<FStat_Vigor>(FName(*RowName), TEXT(""));

		if (VigorDTRow)
		{
			Poise = VigorDTRow->Poise;
			Resistance = VigorDTRow->Resistance;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Vigor is Unloaded"));
		}
	}
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
		Delta *= (1.0f - (MeleeDefense / (MeleeDefense + 100.0f)));
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

FCharacterStats UStatComponent::GetBaseStatLevel_Implementation() const
{
	return BaseStats;
}

float UStatComponent::GetStatRequirementRatio_Implementation(const FCharacterStats& RequireStats) const
{
	return BaseStats.GetRequirementStatRate(RequireStats);
}

float UStatComponent::GetWeaponPerformanceRatio_Implementation(const FCharacterStats& RequireStats) const
{
	const float StatFulfillRatio = BaseStats.GetRequirementStatRate(RequireStats);

	if (StatFulfillRatio >= 1.0f)
		return 1.0f;
	else if (StatFulfillRatio >= 0.8f)
		return 0.5f;
	else if (StatFulfillRatio >= 0.5f)
		return 0.3f;
	else
		return 0.2f;
}
