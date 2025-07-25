// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Components/CharacterStatusComponent.h"

#include "Characters/Player/Interfaces/PlayerStatusInterface.h"

#include "PlayerStatusComponent.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API UPlayerStatusComponent : public UCharacterStatusComponent
	, public IPlayerStatusInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerStatusComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

#pragma region Ground
public:
	FORCEINLINE bool IsDodging() const { return GroundState == ECharacterGroundState::Dodge; }
	FORCEINLINE bool IsBlocking() const { return GroundState == ECharacterGroundState::Block; }
	FORCEINLINE bool IsParrying() const { return GroundState == ECharacterGroundState::Parry; }
	FORCEINLINE bool IsInvincible() const { return GroundState == ECharacterGroundState::Invincible; }
	

	bool CanTransition(const ECharacterGroundState NewState) const;

	ECharacterGroundState GetGroundState_Implementation() const { return GroundState; }

	FORCEINLINE ECharacterGroundState GetGroundState_Native() const { return GroundState; }

	void SetCombatState(ECharacterGroundState NewState);

private:
	ECharacterGroundState GroundState;

#pragma endregion Ground
};
