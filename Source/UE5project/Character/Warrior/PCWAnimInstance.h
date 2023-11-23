// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../PEnumHeader.h"
#include "Animation/AnimInstance.h"
#include "PCWAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnEndAttackDelegate);
DECLARE_MULTICAST_DELEGATE(FOnStartHitDelegate);
DECLARE_MULTICAST_DELEGATE(FOnEndHitDelegate);
/**
 * 
 */
UCLASS()
class UE5PROJECT_API UPCWAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPCWAnimInstance();
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayMontage(MontageType Type, int32 Section = 0);
	void JumpToAttackMontageSection(int32 NewSection);
	MontageType CheckMontage(UAnimMontage* Montage);

public:
	FOnNextAttackCheckDelegate OnNextAttackCheck;
	FOnEndAttackDelegate OnEndAttack;
	FOnStartHitDelegate OnStartHit;
	FOnEndHitDelegate OnEndHit;

private:
	FVector PrevLoc;
	FVector NextLoc;

	class APCWarrior* Character = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Classes, Meta = (AllowPrivateAccess = true))
		class UAnimInstance* Warrior_AnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		float CurrentSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		bool IsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		bool IsAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		bool IsHeavyAttack;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Character, Meta = (AllowPrivateAccess = true))
		bool CombatMode;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Character, Meta = (AllowPrivateAccess = true))
		bool IsTurning=false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		int32 ComboCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		float Direction;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Character, Meta = (AllowPrivateAccess = true))
		float Pitch = 0.0f;;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Character, Meta = (AllowPrivateAccess = true))
		float Yaw = 0.0f;;



	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TIP, Meta = (AllowPrivateAccess = true))
		float RootYawOffset = 0.0f;;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TIP, Meta = (AllowPrivateAccess = true))
		float MaxTurnAngle = 90.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TIP, Meta = (AllowPrivateAccess = true))
		float DistanceCurveValue = -90.0f;;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TIP, Meta = (AllowPrivateAccess = true))
		float D = 0.0f;;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TIP, Meta = (AllowPrivateAccess = true))
		FName DistanceCurve = TEXT("DistanceToPivot");

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TIP, Meta = (AllowPrivateAccess = true))
		FName Turning = TEXT("Turning");


	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Combat, Meta = (AllowPrivateAccess = true))
		UAnimMontage* AttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Combat, Meta = (AllowPrivateAccess = true))
		UAnimMontage* AttackMontage1;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Combat, Meta = (AllowPrivateAccess = true))
		UAnimMontage* AttackMontage2;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Combat, Meta = (AllowPrivateAccess = true))
		UAnimMontage* AttackMontage3;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Combat, Meta = (AllowPrivateAccess = true))
		UAnimMontage* AttackMontage4;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Combat, Meta = (AllowPrivateAccess = true))
		UAnimMontage* DeathMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Combat, Meta = (AllowPrivateAccess = true))
		UAnimMontage* DodgeMontage;

	UFUNCTION()
		void AnimNotify_NOT_NextAttack();

	UFUNCTION()
		void AnimNotify_NOT_EndAttack();

	UFUNCTION()
		void AnimNotify_NOT_StartHit();

	UFUNCTION()
		void AnimNotify_NOT_EndHit();

	FName GetAttackMontageSectionName(int32 Section);

private:
	void SetPitch();
	void SetRootYawOffset();

protected:
	float YawLastTick = 0.0f;
	float YawChangeOverFrame = 0.0f;
	float ABSRootYawOffset = 0.0f;
	float DistanceCurveValueLastFrame = 0.0f;
	float DistanceCurveDifference = 0.0f;
	float YawToSubtract = 0.0f;
	float YawMultiplier = 0.0f;
	float TurnDirection = 0.0f;
	bool IsMove = false;
};