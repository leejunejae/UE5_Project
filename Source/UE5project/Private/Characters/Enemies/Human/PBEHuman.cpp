// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/Human/PBEHuman.h"

// 이동
#include "GameFramework/CharacterMovementComponent.h"

// 컴포넌트
#include "Characters/Components/StatComponent.h"
#include "Combat/Components/AttackComponent.h"

// 콜리전
#include "Components/CapsuleComponent.h"

APBEHuman::APBEHuman()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = GetCapsuleComponent();

	AttackComponent = CreateDefaultSubobject<UAttackComponent>(TEXT("AttackComponent"));
	AttackComponent->bAutoActivate = true;

	Statcomponent = CreateDefaultSubobject<UStatComponent>(TEXT("Statcomponent"));
	Statcomponent->bAutoActivate = true;

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
}

void APBEHuman::BeginPlay()
{
	Super::BeginPlay();

	if (Statcomponent != nullptr)
	{
		Statcomponent->OnDeath.BindUFunction(this, FName("Death"));
	}
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

UStaticMeshComponent* APBEHuman::GetWeapon_Implementation()
{
	return Weapon;
}

/*
void APBEHuman::TakeDamage_Implementation(FAttackInfo DamageInfo)
{
	IHitReactionInterface::TakeDamage_Implementation(DamageInfo);

	return statcomponent->TakeDamage(DamageInfo);
}

float APBEHuman::GetMaxHealth_Implementation()
{
	IHitReactionInterface::GetMaxHealth_Implementation();

	return statcomponent->GetfloatValue("MaxHealth");
}

float APBEHuman::GetHealth_Implementation()
{
	IHitReactionInterface::GetHealth_Implementation();

	return 0.0f;//statcomponent->GetfloatValue("Health");
}

float APBEHuman::Heal_Implementation(float amount)
{
	IHitReactionInterface::Heal_Implementation(amount);

	return statcomponent->Heal(amount);
}
*/