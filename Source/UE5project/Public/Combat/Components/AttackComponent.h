// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataAsset.h"

#include "Combat/Data/DataAsset/PlayerAttackDataAsset.h"
#include "AttackComponent.generated.h"

class UPBEHAnimInstance;
class UHitReactionInterface;
class UNiagaraSystem;
class UNiagaraComponent;
class IAttackSourceInterface;

DECLARE_DELEGATE(FOnSingleDelegate);

struct FWeaponSegment {
	FVector StartWS;
	FVector EndWS;

	FWeaponSegment() {}
	FWeaponSegment(const FVector& InStart, const FVector& InEnd)
		: StartWS(InStart), EndWS(InEnd)
	{}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5PROJECT_API UAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttackComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//void SetMovementMode();

public:	
	FORCEINLINE void SetAttackDA(const UDataAsset* AttackDA) { AttackListDA = AttackDA; }

	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	virtual void ExecuteAttack(FName AttackName, float Playrate = 1.0f);
	virtual void PlayAnimation(FAttackContext AttackInfo, int32 index, float Playrate = 1.0f);
	virtual void DetectAttackTarget(float StartTime, float EndTime);
	void ResetTraceData();

	FTransform SamplingBoneTransformAnimTime(UAnimSequence* AnimSequence, const FName& BoneName, float Time);

protected:
	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = true))
		const class UDataAsset* AttackListDA;

	TScriptInterface<IAttackSourceInterface> AttackSourceInterface;

	FOnMontageEnded OnMontageEndedDelegate;

	TTuple<bool, float> DetectTracePrevTime;

	TSet<AActor*> HitActorListCurrentAttack;

	UPROPERTY(VisibleAnywhere, Category = Attack)
		FAttackContextSet CurAttackContextSet;

	UPROPERTY(VisibleAnywhere, Category = Attack)
		FAttackContext CurAttackContext;

	UPROPERTY(VisibleAnywhere, Category = Attack)
		int32 ComboIndex = 0;
};
