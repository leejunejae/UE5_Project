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

public:
	FORCEINLINE bool IsDead() const { return bIsDead; }

	bool IsInAir() const;

	void HandleDeath();

	FOnMultiDelegate OnDeath;

protected:
	TWeakObjectPtr<ACharacter> CachedCharacter;

	bool bIsDead = false;
};
