// Fill out your copyright notice in the Description page of Project Settings.


#include "PCWarrior.h"

APCWarrior::APCWarrior()
{
	PrimaryActorTick.bCanEverTick = true;


	static ConstructorHelpers::FObjectFinder<USkeletalMesh>PBC_Mannequin(TEXT("/Game/Character/C_Mesh/CM_Warrior/Paladin_WProp_J_Nordstrom.Paladin_WProp_J_Nordstrom"));
	if (PBC_Mannequin.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(PBC_Mannequin.Object);
	}
}

void APCWarrior::BeginPlay()
{
	Super::BeginPlay();
}

void APCWarrior::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APCWarrior::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}