// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Characters/Data/StatusData.h"
#include "Characters/Interfaces/CharacterStatusInterface.h"
#include "CharacterStatusComponent.generated.h"

class ACharacter;
class UCharacterMovementComponent;

DECLARE_MULTICAST_DELEGATE(FOnMultiDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5PROJECT_API UCharacterStatusComponent : public UActorComponent
	, public ICharacterStatusInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterStatusComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	FORCEINLINE bool IsDead() const { return bIsDead; }

	bool IsInAir() const;

	void ExecuteDeath();

	FOnMultiDelegate OnDeath;

protected:
	TWeakObjectPtr<ACharacter> CachedCharacter;

	bool bIsDead = false;

#pragma region State
protected:
	UPROPERTY(VisibleAnywhere, Category = "State")
	ECharacterState CurrentState = ECharacterState::Ground;

public:
	FORCEINLINE ECharacterState GetCharacterState_Native() const { return CurrentState; }
	FORCEINLINE void SetCharacterState_Native(ECharacterState NewState) { CurrentState = NewState; }

	ECharacterState GetCharacterState_Implementation() const { return CurrentState; }
	void SetCharacterState_Implementation(ECharacterState NewState) { CurrentState = NewState; }
#pragma endregion State


#pragma region Ground
public:
	FORCEINLINE bool IsDodging() const { return GroundStance == EGroundStance::Dodge; }
	FORCEINLINE bool IsBlocking() const { return GroundStance == EGroundStance::Block; }
	FORCEINLINE bool IsParrying() const { return GroundStance == EGroundStance::Parry; }
	FORCEINLINE bool IsInvincible() const { return GroundStance == EGroundStance::Invincible; }

	FORCEINLINE EGroundStance GetGroundStance_Native() const { return GroundStance; }
	FORCEINLINE void SetGroundStance_Native(EGroundStance NewStance) { GroundStance = NewStance; }

	EGroundStance GetGroundStance_Implementation() const { return GroundStance; }
	void SetGroundStance_Implementation(EGroundStance NewStance) { GroundStance = NewStance; }

	bool CanTransitionGroundStance(EGroundStance DestStance, EGroundStance TargetStance);
private:
	UPROPERTY(VisibleAnywhere, Category = "Stance")
	EGroundStance GroundStance;
#pragma endregion Ground
};
