// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/Human/PBEHuman.h"

// 이동
#include "GameFramework/CharacterMovementComponent.h"

// 컴포넌트
#include "Characters/Components/StatComponent.h"
#include "Combat/Components/AttackComponent.h"

// 콜리전
#include "Components/CapsuleComponent.h"

APBEHuman::APBEHuman(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = GetCapsuleComponent();

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