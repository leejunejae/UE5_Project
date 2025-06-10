// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../StatusData.h"
#include "CharacterStatusComponent.generated.h"

class ACharacter;
class UCharacterMovementComponent;

DECLARE_MULTICAST_DELEGATE(FOnMultiDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5PROJECT_API UCharacterStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterStatusComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

#pragma region Combat
public:
	FORCEINLINE bool IsDodging() const { return CombatState == ECharacterCombatState::Dodge; }
	FORCEINLINE bool IsBlocking() const { return CombatState == ECharacterCombatState::Block; }
	FORCEINLINE bool IsParrying() const { return CombatState == ECharacterCombatState::Parry; }
	FORCEINLINE bool IsInvincible() const { return CombatState == ECharacterCombatState::Invincible; }
	FORCEINLINE bool IsDead() const { return bIsDead; }
	bool IsInAir() const;

	void HandleDeath();
	
	bool CanTransition(const ECharacterCombatState NewState) const;

	ECharacterCombatState GetCombatState() const { return CombatState; }

	void SetCombatState(ECharacterCombatState NewState);

public:
	FOnMultiDelegate OnDeath;

private:
	ECharacterCombatState CombatState;

	bool bIsDead = false;
#pragma endregion Combat
};
