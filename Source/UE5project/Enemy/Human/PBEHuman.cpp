// Fill out your copyright notice in the Description page of Project Settings.


#include "PBEHuman.h"
#include <Engine/Classes/Components/CapsuleComponent.h>
#include "GameFramework/CharacterMovementComponent.h"

APBEHuman::APBEHuman()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = GetCapsuleComponent();
	DamageSystem = CreateDefaultSubobject<UPBDamageSystem>(TEXT("DAMAGESYSTEM"));
	DamageSystem->bAutoActivate = true;
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Character"));

	//bUseControllerRotationYaw = false;
	//GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 0.0f;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 180.0f);

	Tags.Add("Enemy");
}

void APBEHuman::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	DamageSystem->OnDeath.BindUFunction(this, FName("Death"));
	DamageSystem->OnBlocked.BindUFunction(this, FName("Block"));
	DamageSystem->OnDamaged.BindUFunction(this, FName("DamageResponse"));
}

void APBEHuman::Attack()
{
	UE_LOG(LogTemp, Error, TEXT("Attack"));
}

void APBEHuman::SetMovementSpeed(float speed)
{
	GetCharacterMovement()->MaxWalkSpeed = speed;
}

FRangeInfo APBEHuman::GetIdealRange()
{
	FRangeInfo ReturnRange;
	ReturnRange.Melee = MeleeRadius;
	ReturnRange.Ranged = RangedRadius;
	ReturnRange.Defend = DefendRadius;

	return ReturnRange;
}

void APBEHuman::Death()
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy Dead"));
	IsDead = true;
	SetActorEnableCollision(false);
}

void APBEHuman::Block(bool CanParried)
{
	if (CanParried)
	{
		UE_LOG(LogTemp, Warning, TEXT("Your Character Parried"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Your Character Blocked"));
	}
}

void APBEHuman::DamageResponse(HitResponse Response)
{
	UE_LOG(LogTemp, Warning, TEXT("Your Character Was Damaged"));
}

bool APBEHuman::TakeDamage_Implementation(FDamageInfo DamageInfo)
{
	IPBDamagableInterface::TakeDamage_Implementation(DamageInfo);

	return DamageSystem->TakeDamage(DamageInfo);
}

float APBEHuman::GetMaxHealth_Implementation()
{
	IPBDamagableInterface::GetMaxHealth_Implementation();

	return DamageSystem->GetfloatValue("MaxHealth");
}

float APBEHuman::GetHealth_Implementation()
{
	IPBDamagableInterface::GetHealth_Implementation();

	return DamageSystem->GetfloatValue("Health");
}

float APBEHuman::Heal_Implementation(float amount)
{
	IPBDamagableInterface::Heal_Implementation(amount);

	return DamageSystem->Heal(amount);
}