// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/StatComponent.h"
#include "Characters/Components/CharacterStatusComponent.h"

// Sets default values for this component's properties
UStatComponent::UStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UDataTable> VitalityDT_Asset(TEXT("DataTable'/Game/00_Character/Data/Stat/Attribute_Vitality_DT.Attribute_Vitality_DT'"));
	if (VitalityDT_Asset.Succeeded())
	{
		AttributeTables.Add(EAttributeType::Vitality, VitalityDT_Asset.Object);
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> EnduranceDT_Asset(TEXT("DataTable'/Game/00_Character/Data/Stat/Attribute_Endurance_DT.Attribute_Endurance_DT'"));
	if (EnduranceDT_Asset.Succeeded())
	{
		AttributeTables.Add(EAttributeType::Endurance, EnduranceDT_Asset.Object);
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> MentalityDT_Asset(TEXT("DataTable'/Game/00_Character/Data/Stat/Attribute_Mentality_DT.Attribute_Mentality_DT'"));
	if (MentalityDT_Asset.Succeeded())
	{
		AttributeTables.Add(EAttributeType::Mentality, MentalityDT_Asset.Object);
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> StrengthDT_Asset(TEXT("DataTable'/Game/00_Character/Data/Stat/Attribute_Strength_DT.Attribute_Strength_DT'"));
	if (StrengthDT_Asset.Succeeded())
	{
		AttributeTables.Add(EAttributeType::Strength, StrengthDT_Asset.Object);
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> DexterityDT_Asset(TEXT("DataTable'/Game/00_Character/Data/Stat/Attribute_Dexterity_DT.Attribute_Dexterity_DT'"));
	if (DexterityDT_Asset.Succeeded())
	{
		AttributeTables.Add(EAttributeType::Dexterity, DexterityDT_Asset.Object);
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> IntelligenceDT_Asset(TEXT("DataTable'/Game/00_Character/Data/Stat/Attribute_Intelligence_DT.Attribute_Intelligence_DT'"));
	if (IntelligenceDT_Asset.Succeeded())
	{
		AttributeTables.Add(EAttributeType::Intelligence, IntelligenceDT_Asset.Object);
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> VigorDT_Asset(TEXT("DataTable'/Game/00_Character/Data/Stat/Attribute_Vigor_DT.Attribute_Vigor_DT'"));
	if (VigorDT_Asset.Succeeded())
	{
		AttributeTables.Add(EAttributeType::Vigor, VigorDT_Asset.Object);
	}
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
	if (AttributeTables.Contains(EAttributeType::Vitality))
	{
		FString RowName = FString::Printf(TEXT("Vitality_%d"), BaseAttributes.Vitality);
		const FAttribute_Vitality* VitalityDTRow = AttributeTables[EAttributeType::Vitality]->FindRow<FAttribute_Vitality>(FName(*RowName), TEXT(""));
		
		if (VitalityDTRow)
		{
			BaseStats.MaxHealth = VitalityDTRow->HP;
			BaseStats.Health = BaseStats.MaxHealth;
		}
		else 
		{
			UE_LOG(LogTemp, Warning, TEXT("Vitality is Unloaded"));
		}
	}

	if (AttributeTables.Contains(EAttributeType::Endurance))
	{
		FString RowName = FString::Printf(TEXT("Endurance_%d"), BaseAttributes.Endurance);
		const FAttribute_Endurance* EnduranceDTRow = AttributeTables[EAttributeType::Endurance]->FindRow<FAttribute_Endurance>(FName(*RowName), TEXT(""));

		if (EnduranceDTRow)
		{
			BaseStats.MaxStamina = EnduranceDTRow->Stamina;
			BaseStats.Stamina = BaseStats.MaxStamina;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Endurance is Unloaded"));
		}
	}

	if (AttributeTables.Contains(EAttributeType::Mentality))
	{
		FString RowName = FString::Printf(TEXT("Mentality_%d"), BaseAttributes.Mentality);
		const FAttribute_Mentality* MentalityDTRow = AttributeTables[EAttributeType::Mentality]->FindRow<FAttribute_Mentality>(FName(*RowName), TEXT(""));

		if (MentalityDTRow)
		{
			BaseStats.MaxFocus = MentalityDTRow->FP;
			BaseStats.Focus = BaseStats.MaxFocus;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Mentality is Unloaded"));
		}
	}

	if (AttributeTables.Contains(EAttributeType::Strength))
	{
		FString RowName = FString::Printf(TEXT("Strength_%d"), BaseAttributes.Strength);
		const FAttribute_Strength* StrengthDTRow = AttributeTables[EAttributeType::Strength]->FindRow<FAttribute_Strength>(FName(*RowName), TEXT(""));

		if (StrengthDTRow)
		{
			BaseStats.MeleeDefense = StrengthDTRow->MeleeDefense;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Strength is Unloaded"));
		}
	}

	if (AttributeTables.Contains(EAttributeType::Dexterity))
	{
		FString RowName = FString::Printf(TEXT("Dexterity_%d"), BaseAttributes.Dexterity);
		const FAttribute_Dexterity* DexterityDTRow = AttributeTables[EAttributeType::Dexterity]->FindRow<FAttribute_Dexterity>(FName(*RowName), TEXT(""));

		if (DexterityDTRow)
		{
			BaseStats.RangedDefense = DexterityDTRow->RangedDefense;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Dexterity is Unloaded"));
		}
	}

	if (AttributeTables.Contains(EAttributeType::Intelligence))
	{
		FString RowName = FString::Printf(TEXT("Intelligence_%d"), BaseAttributes.Intelligence);
		const FAttribute_Intelligence* IntelligenceDTRow = AttributeTables[EAttributeType::Intelligence]->FindRow<FAttribute_Intelligence>(FName(*RowName), TEXT(""));

		if (IntelligenceDTRow)
		{
			BaseStats.MagicDefense = IntelligenceDTRow->MagicDefense;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Intelligence is Unloaded"));
		}
	}

	if (AttributeTables.Contains(EAttributeType::Vigor))
	{
		FString RowName = FString::Printf(TEXT("Vigor_%d"), BaseAttributes.Vigor);
		const FAttribute_Vigor* VigorDTRow = AttributeTables[EAttributeType::Vigor]->FindRow<FAttribute_Vigor>(FName(*RowName), TEXT(""));

		if (VigorDTRow)
		{
			BaseStats.MaxPoise = VigorDTRow->Poise;
			BaseStats.Poise = BaseStats.MaxPoise;
			BaseStats.Resistance = VigorDTRow->Resistance;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Vigor is Unloaded"));
		}
	}
}

void UStatComponent::ChangeMaxHealth(const float Amount)
{
	BaseStats.MaxHealth += Amount;
}

void UStatComponent::ChangeMaxStamina(const float Amount)
{
	BaseStats.MaxStamina += Amount;
}

void UStatComponent::ChangeMaxPoise(const float Amount)
{
	BaseStats.MaxPoise += Amount;
}

bool UStatComponent::ApplyDamage(const float Amount, const EAttackType AttackType)
{
	float Delta = Amount;
	switch (AttackType)
	{
	case EAttackType::PhysicalDamage:
		Delta *= (1.0f - (BaseStats.MeleeDefense / (BaseStats.MeleeDefense + 100.0f)));
		break;
	case EAttackType::MagicalDamage:
		Delta *= (1.0f - (BaseStats.MagicDefense / (BaseStats.MagicDefense + 100.0f)));
		break;
	case EAttackType::TrueDamage:
		break;
	}

	BaseStats.Health = FMath::Clamp(BaseStats.Health - Delta, 0.0f, BaseStats.MaxHealth);
	
	if (BaseStats.Health <= 0.0f)
	{
		if (UCharacterStatusComponent* StatusComp = GetOwner()->FindComponentByClass<UCharacterStatusComponent>())
		{
			StatusComp->ExecuteDeath();
		}
		return false;
	}

	return true;
}

bool UStatComponent::Heal(const float Amount)
{
	BaseStats.Health = FMath::Clamp(BaseStats.Health + Amount, 0.0f, BaseStats.MaxHealth);

	return true;
}

bool UStatComponent::ChangeStamina(const float Amount, const EStatChangeType SPChangeType)
{
	float Delta = Amount;
	bool bChangeSuccess;

	switch (SPChangeType)
	{
	case EStatChangeType::Damage:
		bChangeSuccess = BaseStats.Stamina >= Delta;
		break;
	case EStatChangeType::Heal:
		bChangeSuccess = (BaseStats.Stamina + Delta) <= BaseStats.MaxStamina;
		Delta *= -1.0f;
		break;
	}

	BaseStats.Stamina = FMath::Clamp(BaseStats.Stamina - Delta, 0.0f, BaseStats.MaxStamina);

	return bChangeSuccess;
}

bool UStatComponent::ChangePoise(const float Amount, const EStatChangeType PoiseChangeType)
{
	float Delta = Amount;

	switch (PoiseChangeType)
	{
	case EStatChangeType::Damage:
		break;
	case EStatChangeType::Heal:
		Delta *= -1.0f;
		break;
	}

	BaseStats.Poise = FMath::Clamp(BaseStats.Poise - Delta, 0.0f, BaseStats.MaxPoise);

	if (BaseStats.Poise <= 0.0f || BaseStats.Poise <= BaseStats.MaxPoise)
	{
		BaseStats.Poise = BaseStats.MaxPoise;
		return false;
	}
	else return true;
}

float UStatComponent::GetAttributesRequirementRatio_Implementation(const FCharacterAttributes& RequireStats) const
{
	return BaseAttributes.GetRequirementAttributeRate(RequireStats);
}

float UStatComponent::GetWeaponPerformanceRatio_Implementation(const FCharacterAttributes& RequireStats) const
{
	const float StatFulfillRatio = BaseAttributes.GetRequirementAttributeRate(RequireStats);

	if (StatFulfillRatio >= 1.0f)
		return 1.0f;
	else if (StatFulfillRatio >= 0.8f)
		return 0.5f;
	else if (StatFulfillRatio >= 0.5f)
		return 0.3f;
	else
		return 0.2f;
}
