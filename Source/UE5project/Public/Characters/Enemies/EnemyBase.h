// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NavigationSystem.h"
#include "NavigationInvokerComponent.h"

// 인터페이스
#include "Combat/Interfaces/HitReactionInterface.h"
#include "Characters/Interfaces/DeathInterface.h"

// 구조체, 자료형
#include "Characters/Enemies/Data/EnemyData.h"
#include "Combat/Data/CombatData.h"

#include "Characters/Player/CharacterBase.h"
#include "EnemyBase.generated.h"

class UAttackComponent;
class UHitReactionComponent;
class UCharacterStatusComponent;
class AEnemyBaseAIController;

DECLARE_DELEGATE(FOnSingleDelegate);

UCLASS()
class UE5PROJECT_API AEnemyBase : public ACharacterBase,
	public IDeathInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Attack(FName AttackName, ACharacter* Target = nullptr);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
		class UNavigationInvokerComponent* NavigationInvokerComponent;

/* 이 적이 사용할 데이터 */
#pragma region Character_Data
public:
	// 데이터 로드에 사용할 식별자
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BaseData")
		FName EnemyID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseData")
		FEnemyRuntimeStats Stats;

	void InitializeEnemyData();
	bool ValidateData(const FEnemyDataSet* EnemyData);
#pragma endregion Character_Data

#pragma region Stat
public:
	bool ApplyDamage(const float Amount, const EAttackType Attackype);
	bool ChangeStance(const float Amount, const EStatChangeType StatChangeType);
	bool ChangePoise(const float Amount, const EStatChangeType StatChangeType);

#pragma endregion Stat


#pragma region HitReaction
public:
	void OnHit_Implementation(const FAttackRequest& AttackInfo) override;
	
#pragma endregion HitReaction

#pragma region Status
public:
	FOnDeathDelegate OnDeath;
	virtual FOnDeathDelegate& GetOnDeathDelegate() override { return OnDeath; }

#pragma endregion Status

#pragma region AI
protected:
	virtual void PossessedBy(AController* NewController) override;
	
	UPROPERTY(VisibleAnywhere, Category = AI)
	TObjectPtr<AEnemyBaseAIController> CachedAIController;
#pragma endregion
};
