// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NavigationSystem.h"
#include "NavigationInvokerComponent.h"

// 인터페이스
#include "Combat/Interfaces/HitReactionInterface.h"

#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"

class UAttackComponent;
class UHitReactionComponent;

UCLASS()
class UE5PROJECT_API AEnemyBase : public ACharacter,
	public IHitReactionInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Attack(FName AttackName, ACharacter* Target = nullptr);


	bool IsAttack;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(EditAnywhere)
		class UNavigationInvokerComponent* NavigationInvokerComponent;

#pragma region HitReaction
protected:
	UPROPERTY(VisibleAnywhere, Category = HitReaction)
		TObjectPtr<UHitReactionComponent> HitReactionComponent;

public:
	virtual void OnHit_Implementation(const FAttackRequest& AttackInfo) override;

	FORCEINLINE UHitReactionComponent* GetHitReactionComponent() const { return HitReactionComponent; }
	
#pragma endregion HitReaction
};
