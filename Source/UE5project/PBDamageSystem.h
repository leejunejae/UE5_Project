// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PEnumHeader.h"
#include "PBDamageSystem.generated.h"

DECLARE_DELEGATE(FOnDeathDelegate);
DECLARE_DELEGATE_OneParam(FOnBlockedDelegate, bool);
DECLARE_DELEGATE_OneParam(FOnDamagedDelegate, HitResponse);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5PROJECT_API UPBDamageSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPBDamageSystem();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	FOnDeathDelegate OnDeath;
	FOnBlockedDelegate OnBlocked;
	FOnDamagedDelegate OnDamaged;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
		float Heal(float amount);

	UFUNCTION()
		bool TakeDamage(FDamageInfo DamageInfo);

	UFUNCTION()
		void CanbeDamaged(bool ShouldInvincible, bool CanbeBlocked);

	// 변수 호츌 함수
	float GetfloatValue(FString value);

private:
	float Health = 50.0f;
	float MaxHealth = 50.0f;
	bool IsDead;
	bool IsInvincible;
	bool IsInterruptible;
	bool IsBlocking;
	HitCheck CurHitCheck;
};
