// Fill out your copyright notice in the Description page of Project Settings.


#include "PBEnemy.h"
#include "PBEnemyAIController.h"

#include <Engine/Classes/Components/CapsuleComponent.h>

// Sets default values
APBEnemy::APBEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = GetCapsuleComponent();

	NavigationInvokerComponent = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("NavigationInvokerComponent"));
	NavigationInvokerComponent->SetGenerationRadii(500.0f, 500.0f);

	IsAttack = false;
}

// Called when the game starts or when spawned
void APBEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APBEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APBEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APBEnemy::Attack(FName AttackName, ACharacter* Target)
{
	
}