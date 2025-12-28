// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NavigationSystem.h"
//#include "NavigationInvokerComponent.h"

// 인터페이스
#include "Combat/Interfaces/HitReactionInterface.h"
#include "Characters/Interfaces/DeathInterface.h"

// 구조체, 자료형
#include "Characters/Enemies/Data/EnemyData.h"
#include "Combat/Data/CombatData.h"

#include "Characters/CharacterBase.h"
#include "EnemyBase.generated.h"

class UAttackComponent;
class UHitReactionComponent;
class UCharacterStatusComponent;
class UEnemyBaseAnimInstance;
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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

#pragma region Status
public:
	FOnDeathDelegate OnDeath;
	virtual FOnDeathDelegate& GetOnDeathDelegate() override { return OnDeath; }

#pragma endregion Status

//private:
	//UPROPERTY(EditAnywhere)
		//class UNavigationInvokerComponent* NavigationInvokerComponent;

/* 이 적이 사용할 데이터*/
#pragma region Character_Data
public:
	// 데이터 로드에 사용할 식별자
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BaseData")
		FName EnemyID;

	void InitializeEnemyData();

	void SetEnemyID(const FName ID) { EnemyID = ID; }
	bool ApplyEnemyInfo(const FEnemyInfo* Info);
	bool ApplyEnemyStats(const FEnemyStats* Stat);
	bool ValidateData(const FEnemyDataSet* EnemyData);

#pragma endregion Character_Data

#pragma region Stat
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseData")
		FEnemyRuntimeStats Stats;

public:
	FORCEINLINE FEnemyRuntimeStats GetRuntimeStats() { return Stats; }
	bool ApplyDamage(const float Amount, const EAttackType Attackype);
	bool ChangeStance(const float Amount, const EStatChangeType StatChangeType);
	bool ChangePoise(const float Amount, const EStatChangeType StatChangeType);

#pragma endregion Stat


#pragma region HitReaction
public:
	void OnHit_Implementation(const FAttackRequest& AttackInfo) override;
	
#pragma endregion HitReaction

#pragma region AI
protected:
	virtual void PossessedBy(AController* NewController) override;
	
	UPROPERTY(VisibleAnywhere, Category = AI)
	TObjectPtr<AEnemyBaseAIController> CachedAIController;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Route")
		FGuid DefaultRouteGuid;

public:
	void SetDefaultRouteGuid(const FGuid& InGuid) { DefaultRouteGuid = InGuid; }
	FGuid GetDefaultRouteGuid() const { return DefaultRouteGuid; }
#pragma endregion

#pragma region Animation
protected:
	UPROPERTY(VisibleAnywhere, Category = Animation)
		TObjectPtr<UEnemyBaseAnimInstance> CharacterBaseAnim;

	UPROPERTY(VisibleAnywhere, Category = "Animation")
		TMap<FGameplayTag, FLocomotionAnim> AnimProfiles;

	UPROPERTY(VisibleAnywhere, Category = "Animation")
		FGameplayTag CurrentProfileTag;
#pragma endregion Animation
};
