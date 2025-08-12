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


#pragma region Ground
public:
	FORCEINLINE bool IsDodging() const { return GroundState == ECharacterGroundState::Dodge; }
	FORCEINLINE bool IsBlocking() const { return GroundState == ECharacterGroundState::Block; }
	FORCEINLINE bool IsParrying() const { return GroundState == ECharacterGroundState::Parry; }
	FORCEINLINE bool IsInvincible() const { return GroundState == ECharacterGroundState::Invincible; }

	FORCEINLINE ECharacterGroundState GetGroundState_Native() const { return GroundState; }
	FORCEINLINE void SetGroundState_Native(ECharacterGroundState NewState) { GroundState = NewState; }

	ECharacterGroundState GetGroundState_Implementation() const { return GroundState; }
	void SetGroundState_Implementation(ECharacterGroundState NewState) { GroundState = NewState; }

private:
	ECharacterGroundState GroundState;
#pragma endregion Ground
};
