// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// 기본 헤더
#include "CoreMinimal.h"

#include "Interaction/Climb/Data/ClimbHeader.h"

#include "Combat/Interfaces/HitReactionInterface.h"

#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"


class UAttackComponent;
class UHitReactionComponent;
class UCharacterStatusComponent;
class UClimbComponent;

UCLASS()
class UE5PROJECT_API ACharacterBase : public ACharacter,
	public IHitReactionInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;


#pragma region Attack
protected:
	UPROPERTY(VisibleAnywhere, Category = Attack)
		TObjectPtr<UAttackComponent> AttackComponent;

public:
	FORCEINLINE UAttackComponent* GetAttackComponent() const { return AttackComponent; }

#pragma endregion Attack


#pragma region HitReaction
protected:
	UPROPERTY(VisibleAnywhere, Category = HitReaction)
		TObjectPtr<UHitReactionComponent> HitReactionComponent;

public:
	//virtual void OnHit_Implementation(const FAttackRequest& AttackInfo) override;

	FORCEINLINE UHitReactionComponent* GetHitReactionComponent() const { return HitReactionComponent; }

#pragma endregion HitReaction


#pragma region Status
protected:
	UPROPERTY(VisibleAnywhere, Category = Status)
		TObjectPtr<UCharacterStatusComponent> CharacterStatusComponent;

public:
	FORCEINLINE UCharacterStatusComponent* GetCharacterStatusComponent() const { return CharacterStatusComponent; }
#pragma endregion Status

#pragma region Ladder
protected:
	UPROPERTY(VisibleAnywhere, Category = Climb)
		TObjectPtr<UClimbComponent> ClimbComponent;

	UPROPERTY(EditAnywhere)
		float MinGripInterval = 15.0f;
	UPROPERTY(EditAnywhere)
		float MaxGripInterval = 60.0f;
	UPROPERTY(EditAnywhere)
		float MinFirstGripHeight = 0.0f;

public:
	FORCEINLINE UClimbComponent* GetClimbComponent() const { return ClimbComponent; }
#pragma endregion Ladder
};