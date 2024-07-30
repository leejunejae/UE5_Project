// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../PEnumHeader.h"
#include "../../Function/Combat/CombatStruct.h"
#include "Animation/AnimInstance.h"
#include "PCWAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnActionDelegate);
/**
 * 
 

USTRUCT(BlueprintType)
struct FFootTraceStruct
{
	GENERATED_BODY()

		UPROPERTY()
		FRotator Rot;

	UPROPERTY()
		float IK;
};
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
	MontageType CheckMontage(UAnimMontage* Montage);

public:
	FOnActionDelegate OnResetHurt;
	FOnActionDelegate OnSetAttackDirection;
	FOnActionDelegate OnEnterLocomotion;
	FOnActionDelegate OnLeftLocomotion;
	FOnActionDelegate OnAttackStart;
	FOnActionDelegate OnCanDodge;

	FOnActionDelegate OnParryEnd;
	FOnActionDelegate OnNextAttackCheck;
	FOnActionDelegate OnEndAttack;
	FOnActionDelegate OnMountEnd;
	FOnActionDelegate OnDisMountEnd;

	FOnActionDelegate OnDodgeEnd;
	FOnActionDelegate OnDodgeStart;
	FOnActionDelegate OnEquipEnd;
	FOnActionDelegate OnHolsterEnd;

private:
	FVector PrevLoc;
	FVector NextLoc;

	class AWarrior* Character = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Classes, Meta = (AllowPrivateAccess = true))
		class UAnimInstance* Warrior_AnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		bool IsMovementInput;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		float InputDirection;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		bool IsRun;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		bool IsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, Meta = (AllowPrivateAccess = true))
		bool IsAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, Meta = (AllowPrivateAccess = true))
		bool IsAttackInput;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, Meta = (AllowPrivateAccess = true))
		bool IsBlock;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, Meta = (AllowPrivateAccess = true))
		bool IsParry;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		bool IsRoll;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, Meta = (AllowPrivateAccess = true))
		HitResponse Response;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, Meta = (AllowPrivateAccess = true))
		float BlockBlend;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		bool IsRide;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		bool MountRight;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Character, Meta = (AllowPrivateAccess = true))
		bool CombatMode;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Character, Meta = (AllowPrivateAccess = true))
		bool IsTurning=false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		bool IsEscape;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		bool IsRightHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, Meta = (AllowPrivateAccess = true))
		bool CanAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, Meta = (AllowPrivateAccess = true))
		bool IsNextAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		int32 ComboCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		int32 AttackSeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		WarriorStance CurStance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		float LastDirection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		bool IsAccelerating;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		float Displacement = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Character, Meta = (AllowPrivateAccess = true))
		float Pitch = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Character, Meta = (AllowPrivateAccess = true))
		float Yaw = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TIP, Meta = (AllowPrivateAccess = true))
		float RootYawOffset = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TIP, Meta = (AllowPrivateAccess = true))
		float MaxTurnAngle = 90.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TIP, Meta = (AllowPrivateAccess = true))
		float DistanceCurveValue = -90.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TIP, Meta = (AllowPrivateAccess = true))
		FName DistanceCurve = TEXT("DistanceToPivot");

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TIP, Meta = (AllowPrivateAccess = true))
		FName Turning = TEXT("Turning");

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Combat, Meta = (AllowPrivateAccess = true))
		UAnimMontage* DeathMontage;



	// FootIK Variable for Blueprint
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = FootIK, Meta = (AllowPrivateAccess = true))
		FRotator LeftFootRotator;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = FootIK, Meta = (AllowPrivateAccess = true))
		FRotator RightFootRotator;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = FootIK, Meta = (AllowPrivateAccess = true))
		float PelvisOffset;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = FootIK, Meta = (AllowPrivateAccess = true))
		float LeftFootOffset;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = FootIK, Meta = (AllowPrivateAccess = true))
		float RightFootOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FootIK, Meta = (AllowPrivateAccess = true))
		float PelvisStrength;


	UFUNCTION()
		void AnimNotify_NOT_ParryEnd();

	UFUNCTION()
		void AnimNotify_NOT_NextAttack();

	UFUNCTION()
		void AnimNotify_NOT_MountEnd();

	UFUNCTION()
		void AnimNotify_NOT_DisMountEnd();



	UFUNCTION(BlueprintCallable)
		void AnimNotify_NOT_ResetHurt();

	UFUNCTION(BlueprintCallable)
		void AnimNotify_NOT_EnterLocomotion();

	UFUNCTION(BlueprintCallable)
		void AnimNotify_NOT_LeftLocomotion();

	UFUNCTION(BlueprintCallable)
		void AnimNotify_NOT_DodgeEnd();

	UFUNCTION(BlueprintCallable)
		void AnimNotify_NOT_DodgeStart();

	UFUNCTION(BlueprintCallable)
		void AnimNotify_NOT_SetAttackDirection();

	UFUNCTION(BlueprintCallable)
		void AnimNotify_NOT_AttackStart();

	UFUNCTION()
		void AnimNotify_NOT_CanDodge();

	UFUNCTION(BlueprintCallable)
		void AnimNotify_NOT_EndAttack();

	/*
	UFUNCTION()
		void AnimNotify_NOT_EquipEnd();

	UFUNCTION()
		void AnimNotify_NOT_HolsterEnd();
		*/

	FName GetAttackMontageSectionName(int32 Section);

private:
	void SetPitch();
	void SetRootYawOffset();
	
	
	TTuple<FVector, float> FootTrace(FName SocketName);
	void FootRotation(float DeltaTime, FVector TargetNormal, FRotator *FootRotator, float fInterpSpeed);
	void FootPlacement(float DeltaTime, float TargetValue, float* FootValue, float InterpSpeed);

private:
	float InputDirectionX;
	float InputDirectionY;

	bool SeedSwitch;
	float TargetSpeed;
	float TargetDirection;

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