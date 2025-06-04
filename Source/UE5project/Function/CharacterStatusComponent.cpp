// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStatusComponent.h"
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

	UCharacterStatusComponent* StatusComp = GetOwner()->FindComponentByClass<UCharacterStatusComponent>();
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

bool UCharacterStatusComponent::CanTransition(const ECharacterCombatState NewState) const
{
	switch (CombatState)
	{
	case ECharacterCombatState::Normal:
		return true;
	case ECharacterCombatState::Block:
		return NewState != ECharacterCombatState::Block;
	case ECharacterCombatState::Dodge:
	{
		return NewState == ECharacterCombatState::Invincible;
	}
	case ECharacterCombatState::Parry:
	{
		return NewState == ECharacterCombatState::Invincible;
	}
	case ECharacterCombatState::Invincible:
	{
		return false;
	}
	default:
		return false;
	}
}

void UCharacterStatusComponent::SetCombatState(ECharacterCombatState NewState)
{
	if (!CanTransition(NewState))
		return;

	CombatState = NewState;
}

