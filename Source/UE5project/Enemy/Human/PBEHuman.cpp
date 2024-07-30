// Fill out your copyright notice in the Description page of Project Settings.


#include "PBEHuman.h"
#include <Engine/Classes/Components/CapsuleComponent.h>
#include "GameFramework/CharacterMovementComponent.h"

APBEHuman::APBEHuman()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = GetCapsuleComponent();
	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	CombatComponent->bAutoActivate = true;
	DamageSystem = CreateDefaultSubobject<UPBDamageSystem>(TEXT("DAMAGESYSTEM"));
	DamageSystem->bAutoActivate = true;

	MotionWarpComp = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("UMotionWarpingComponent"));
	MotionWarpComp->bAutoActivate = true;

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));
	GetMesh()->SetCollisionProfileName("Enemy");

	//bUseControllerRotationYaw = false;
	//GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 180.0f);

	Tags.Add("Enemy");
}

void APBEHuman::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	DamageSystem->OnDeath.BindUFunction(this, FName("Death"));
}

void APBEHuman::BeginPlay()
{
	Super::BeginPlay();
	DamageSystem->SetHealth(GetMaxHP());
}

void APBEHuman::Attack(FName AttackName, ACharacter* Target)
{
	
}

void APBEHuman::IsMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	
}

void APBEHuman::OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{

}

void APBEHuman::OnMontageNotifyEnd(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	
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

void APBEHuman::Block(bool IsDefenseMode)
{

}

void APBEHuman::Teleport()
{
	
}

bool APBEHuman::IsLookingAt()
{
	return IsLookAt;
}

AActor* APBEHuman::GetLookingTarget()
{
	return LookAtTarget;
}

void APBEHuman::SetLookingTarget(AActor* Target)
{
	LookAtTarget = Target;
}

void APBEHuman::SetIsLook(bool IsLook)
{
	IsLookAt = IsLook;
}

FString APBEHuman::GetName()
{
	return EnemyInfo.EName;
}

int32 APBEHuman::GetMaxHP()
{
	return EnemyInfo.EMaxHP;
}

int32 APBEHuman::GetOffensePower()
{
	return EnemyInfo.EOffensePower;
}

int32 APBEHuman::GetDefenseCap()
{
	return EnemyInfo.EDefenseCap;
}

int32 APBEHuman::GetDownPercent()
{
	return EnemyInfo.EDownPercent;
}

void APBEHuman::Summon()
{
	
}

void APBEHuman::Appear()
{

}

void APBEHuman::Swoop()
{

}

void APBEHuman::Dash(FVector TargetLocation)
{

}


void APBEHuman::TakeDamage_Implementation(FAttackInfo DamageInfo)
{
	IPBDamagableInterface::TakeDamage_Implementation(DamageInfo);

	return DamageSystem->TakeDamage(DamageInfo);
}

float APBEHuman::GetMaxHealth_Implementation()
{
	IPBDamagableInterface::GetMaxHealth_Implementation();

	return DamageSystem->GetfloatValue("MaxHealth");
}

UStaticMeshComponent* APBEHuman::GetWeapon_Implementation()
{
	return Weapon;
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