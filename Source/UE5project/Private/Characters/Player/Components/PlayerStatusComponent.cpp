// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/Components/PlayerStatusComponent.h"
#include "GameFramework/Character.h" 

// Sets default values for this component's properties
UPlayerStatusComponent::UPlayerStatusComponent()
{

	// ...
}

void UPlayerStatusComponent::BeginPlay()
{
	Super::BeginPlay();

}

bool UPlayerStatusComponent::CanTransition(const ECharacterGroundState NewState) const
{
	switch (GroundState)
	{
	case ECharacterGroundState::Normal:
		return true;
	case ECharacterGroundState::Block:
		return NewState != ECharacterGroundState::Block;
	case ECharacterGroundState::Dodge:
	{
		return NewState == ECharacterGroundState::Invincible;
	}
	case ECharacterGroundState::Parry:
	{
		return NewState == ECharacterGroundState::Invincible;
	}
	case ECharacterGroundState::Invincible:
	{
		return false;
	}
	default:
		return false;
	}
}

void UPlayerStatusComponent::SetCombatState(ECharacterGroundState NewState)
{
	if (!CanTransition(NewState))
		return;

	GroundState = NewState;
}