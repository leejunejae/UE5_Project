// Fill out your copyright notice in the Description page of Project Settings.

// 기본 헤더
#include "Characters/Player/CharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "Characters/Components/BaseCharacterMovementComponent.h"

#include "Combat/Components/AttackComponent.h"
#include "Combat/Components/HitReactionComponent.h"
#include "Characters/Components/CharacterStatusComponent.h"
#include "Interaction/Climb/Components/ClimbComponent.h"

// Sets default values
ACharacterBase::ACharacterBase(const FObjectInitializer& ObjectInitializer)
	: ACharacter(ObjectInitializer.SetDefaultSubobjectClass<UBaseCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = GetCapsuleComponent();

	AttackComponent = CreateDefaultSubobject<UAttackComponent>(TEXT("AttackComponent"));
	AttackComponent->bAutoActivate = true;

	HitReactionComponent = CreateDefaultSubobject<UHitReactionComponent>(TEXT("HitReactionComponent"));
	HitReactionComponent->bAutoActivate = true;

	CharacterStatusComponent = CreateDefaultSubobject<UCharacterStatusComponent>(TEXT("CharacterStatusComponent"));
	CharacterStatusComponent->bAutoActivate = true;

	ClimbComponent = CreateDefaultSubobject<UClimbComponent>(TEXT("ClimbComponent"));
	ClimbComponent->bAutoActivate = true;

	ClimbComponent->SetMinGripInterval(MinGripInterval);
	ClimbComponent->SetMaxGripInterval(MaxGripInterval);
	ClimbComponent->SetMinFirstGripHeight(MinFirstGripHeight);
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}