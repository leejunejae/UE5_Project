// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../StatusData.h"
#include "CharacterStatusComponent.generated.h"


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
	bool IsDodging() const { return CombatState == ECharacterCombatState::Dodge; }
	bool IsBlocking() const { return CombatState == ECharacterCombatState::Block; }
	bool IsParrying() const { return CombatState == ECharacterCombatState::Parry; }
	bool IsInvincible() const { return CombatState == ECharacterCombatState::Invincible; }
	
	bool CanTransition(const ECharacterCombatState NewState) const;


	ECharacterCombatState GetCombatState() const { return CombatState; }

	void SetCombatState(ECharacterCombatState NewState);

private:
	ECharacterCombatState CombatState;
#pragma endregion Combat
};
