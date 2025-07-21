// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Characters/Data/StatusData.h"
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

#pragma region Ground
public:
	FORCEINLINE bool IsDodging() const { return GroundState == ECharacterGroundState::Dodge; }
	FORCEINLINE bool IsBlocking() const { return GroundState == ECharacterGroundState::Block; }
	FORCEINLINE bool IsParrying() const { return GroundState == ECharacterGroundState::Parry; }
	FORCEINLINE bool IsInvincible() const { return GroundState == ECharacterGroundState::Invincible; }
	FORCEINLINE bool IsDead() const { return bIsDead; }
	bool IsInAir() const;

	void HandleDeath();
	
	bool CanTransition(const ECharacterGroundState NewState) const;

	FORCEINLINE ECharacterGroundState GetGroundState() const { return GroundState; }

	void SetCombatState(ECharacterGroundState NewState);

public:
	FOnMultiDelegate OnDeath;

private:
	ECharacterGroundState GroundState;

	bool bIsDead = false;
#pragma endregion Ground
};
