// Fill out your copyright notice in the Description page of Project Settings.


#include "PBDamageSystem.h"

// Sets default values for this component's properties
UPBDamageSystem::UPBDamageSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPBDamageSystem::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPBDamageSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPBDamageSystem::InitailizeInfo(int32 MaxHP, int32 DefenseCap, int32 DownPercent)
{
}

float UPBDamageSystem::Heal(float amount)
{
	if (!IsDead)
	{
		Health = FMath::Clamp(Health + amount, 0.0f, MaxHealth);
		return Health;
	}

	return NULL;
}

float UPBDamageSystem::GetfloatValue(FString value)
{
	if (value == "MaxHealth")
		return MaxHealth;
	else if (value == "Health")
		return Health;
	else
		return NULL;
}

void UPBDamageSystem::TakeDamage(FAttackInfo DamageInfo)
{
	//CanbeDamaged(DamageInfo.Invincible, DamageInfo.CanBlocked);
	DecideResponse(DamageInfo.CanBlocked, DamageInfo.CanAvoid, DamageInfo.CanParried);
	//UE_LOG(LogTemp, Warning, TEXT("Damage path 2"));
	/*
	switch (CurHitCheck)
	{
	case HitCheck::Damaged:
	{
		Health -= DamageInfo.Amount;
		if (Health <= 0.0f)
		{
			IsDead = true;
			OnDeath.ExecuteIfBound();
		}
		else
		{
			if (IsInterruptible || DamageInfo.ForceInterrupt)
			{
				OnDamaged.ExecuteIfBound(DamageInfo.Response);
				UE_LOG(LogTemp, Warning, TEXT("HitLog2"));
			}
			UE_LOG(LogTemp, Warning, TEXT("HitLog3"));
		}
		return true;
	}
	case HitCheck::Blocked:
	{
		OnBlocked.ExecuteIfBound(DamageInfo);
		return false;
	}
	case HitCheck::Invincibled:
		return false;
	}
	*/
}

void UPBDamageSystem::CanbeDamaged(bool ShouldInvincible, bool CanbeBlocked)
{

}

void UPBDamageSystem::DecideResponse(bool CanBlocked, bool CanAvoid, bool CanParry)
{
	//UE_LOG(LogTemp, Warning, TEXT("Damage path 3"));
}

void UPBDamageSystem::SetHealth(int32 InitHealth)
{
	MaxHealth = InitHealth;
	Health = InitHealth;
}

void UPBDamageSystem::SetBlocking(bool Block)
{
	IsBlocking = Block;
}