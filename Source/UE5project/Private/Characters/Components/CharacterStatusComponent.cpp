// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/CharacterStatusComponent.h"
#include "GameFramework/Character.h" 
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UCharacterStatusComponent::UCharacterStatusComponent()
{

	// ...
}


// Called when the game starts
void UCharacterStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	CachedCharacter = Cast<ACharacter>(GetOwner());
	// ...
}

bool UCharacterStatusComponent::IsInAir() const
{
	if (const ACharacter* Character = Cast<ACharacter>(GetOwner()))
	{
		return Character->GetCharacterMovement()->IsFalling();
	}
	return false;
}

void UCharacterStatusComponent::ExecuteDeath()
{
	bIsDead = true;
	OnDeath.Broadcast();
}

bool UCharacterStatusComponent::CanTransitionGroundStance(EGroundStance DestStance, EGroundStance TargetStance)
{

	return false;
}
