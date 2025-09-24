// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataAsset.h"

// 인터페이스
#include "Characters/Interfaces/EquipmentDataInterface.h"

#include "Combat/Data/DataAsset/PlayerAttackDataAsset.h"
#include "AttackComponent.generated.h"

class UPBEHAnimInstance;
class UHitReactionInterface;
class UNiagaraSystem;
class UNiagaraComponent;

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

	UFUNCTION()
		void OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	UFUNCTION()
		void OnMontageNotifyEnd(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	//void SetMovementMode();

public:	
	bool ReCastOwner();
	bool CheckOwnerExist();
	ACharacter* GetMyCharacter();

	FORCEINLINE void SetAttackDA(const UDataAsset* AttackDA) { AttackListDA = AttackDA; }

	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	//void SetCurAttackType(EWeaponType WeaponType);
	virtual void ExecuteAttack(FName AttackName, float Playrate = 1.0f);
	virtual void PlayAnimation(FPlayerAttackInfo AttackInfo, int32 index, float Playrate = 1.0f);
	virtual void DetectAttackTarget(UStaticMeshComponent* WeaponMesh, FWeaponSetsInfo WeaponInfo, float StartTime, float EndTime, bool IsDetectEnd = false, bool IsSubWeaponAttack = false);

	void ExecuteWeaponTrail(FWeaponSetsInfo WeaponInfo, float StartTime, float EndTime, bool IsTrailEnd = false, bool IsSubWeaponTrail = false);

	bool SampleWeaponSegmentAtTime(float Time, const UAnimBoneTransformDataAsset* BoneData, const FWeaponPartInfo& WeaponPart, FWeaponSegment& OutSeg) const;

private:
	FTimerHandle AttackTimerHandle;
	FTimerHandle TrailTimerHandle;

	bool IsDetectAttackTarget;

protected:
	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = true))
		const class UDataAsset* AttackListDA;

	UAnimInstance* AnimInstance;

	TWeakObjectPtr<ACharacter> CachedCharacter;
	TScriptInterface<IEquipmentDataInterface> CachedEquipment;

	FOnMontageEnded OnMontageEndedDelegate;

	TTuple<bool, float> DetectTracePrevTime;

	TTuple<bool, float> TrailPrevTime;

	TSet<AActor*> HitActorListCurrentAttack;

	UPROPERTY(VisibleAnywhere, Category = Effect)
		TObjectPtr<UNiagaraSystem> TrailSystem;

	UPROPERTY(VisibleAnywhere, Category = Effect)
		TObjectPtr<UNiagaraComponent> TrailComponent;

	UPROPERTY(VisibleAnywhere, Category = Attack)
		FPlayerAttackInfoList CurAttackType;

	UPROPERTY(VisibleAnywhere, Category = Attack)
		FPlayerAttackInfo CurAttackInfo;

	UPROPERTY(VisibleAnywhere, Category = Attack)
		int32 ComboIndex = 0;
};
