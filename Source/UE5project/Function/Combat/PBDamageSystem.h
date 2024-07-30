// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../PEnumHeader.h"
#include "CombatStruct.h"
#include "PBDamageSystem.generated.h"

DECLARE_DELEGATE(FOnDeathDelegate);

USTRUCT(BlueprintType)
struct FHitInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* Anim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "MontageVariable != nullptr"))
		FName SectionName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "MontageVariable != nullptr"))
		FText Discrption;

	inline bool operator==(const FHitData& Other) const
	{
		return SectionName == Other.SectionName;
	}
};

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

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
		void InitailizeInfo(int32 MaxHP, int32 DefenseCap, int32 DownPercent=0);

	UFUNCTION()
		float Heal(float amount);

	UFUNCTION()
		void TakeDamage(FAttackInfo AttackInfo);

	UFUNCTION()
		void CanbeDamaged(bool ShouldInvincible, bool CanbeBlocked);

	UFUNCTION()
		void DecideResponse(bool CanBlocked, bool CanAvoid, bool CanParry);

	void SetHealth(int32 InitHealth);

	void SetBlocking(bool Block);

	// ���� ȣ�� �Լ�
	float GetfloatValue(FString value);

private:
	float Health;
	float MaxHealth;
	bool IsDead;
	bool IsInterruptible;
	bool IsInvincible;
	bool IsBlocking;
	bool IsParrying;
	bool IsDodging;
	HitResponse CurHitResponse;
};