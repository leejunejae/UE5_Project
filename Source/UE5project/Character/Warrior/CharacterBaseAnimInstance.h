// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../PEnumHeader.h"
#include "../../BaseCharacterHeader.h"
#include "../../Function/Combat/CombatStruct.h"
#include "../../Function/IAnimInstance.h"
#include "Animation/AnimInstance.h"
#include "CharacterBaseAnimInstance.generated.h"

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
class UE5PROJECT_API UCharacterBaseAnimInstance : public UAnimInstance, public IIAnimInstance
{
	GENERATED_BODY()

public:
	UCharacterBaseAnimInstance();
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

// Ladder Delegate
	FOnActionDelegate OnClimbEnd;

private:
	FVector PrevLoc;
	FVector NextLoc;

	class AFallenKnight* Character = nullptr;

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
		float LastSpeed;

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
#pragma region State & Stance
private:
	UFUNCTION(BlueprintCallable)
		void AnimNotify_NOT_EnterWalkState();
	UFUNCTION(BlueprintCallable)
		void AnimNotify_NOT_EnterLadderState();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
		ECharacterState CurrentState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stance, Meta = (AllowPrivateAccess = true))
		EGroundStance CurGroundStance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stance, Meta = (AllowPrivateAccess = true))
		ELadderStance CurLadderStance;

public:
	FOnActionDelegate OnEnterWalkState;
	FOnActionDelegate OnEnterLadderState;

#pragma region Ground
private:
	UFUNCTION(BlueprintCallable)
		void AnimNotify_NOT_EnableRootLock();
	UFUNCTION(BlueprintCallable)
		void AnimNotify_NOT_DisableRootLock();

#pragma region Movement
	////////////////////////////////////
	// Variables For FootIK
	////////////////////////////////////
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Movement, Meta = (AllowPrivateAccess = true))
		bool CanMovementInput = true;

	UFUNCTION(BlueprintCallable)
		void AnimNotify_NOT_EnterIdleState();

	UFUNCTION(BlueprintCallable)
		void AnimNotify_NOT_LeftIdleState();
#pragma endregion


#pragma region Quick Turn
	void ResetTurn_Implementation() override;
	UFUNCTION(BlueprintCallable)
		void AnimNotify_NOT_TurnStart();
	UFUNCTION(BlueprintCallable)
		void AnimNotify_NOT_TurnEnd();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = QuickTurn, Meta = (AllowPrivateAccess = true))
		bool bQuickTurn;
#pragma endregion


#pragma region Turn In Place

private:
	// Turn In Place // 
	void SetPitch();
	void SetRootYawOffset();

	// Foot IK //
	TTuple<FVector, float> FootTrace(FName SocketName);
	void FootRotation(float DeltaTime, FVector TargetNormal, FRotator* FootRotator, float fInterpSpeed);
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

#pragma endregion

#pragma endregion

#pragma region Ladder
protected:
	//virtual void NativeNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	UFUNCTION(BlueprintCallable)
	void AnimNotify_NOT_CheckLadderStance();

	UFUNCTION(BlueprintCallable)
	void AnimNotify_NOT_EnableInputLock();

	UFUNCTION(BlueprintCallable)
	void AnimNotify_NOT_ClimbEnd();

	UFUNCTION(BlueprintCallable)
	void AnimNotify_NOT_ClimbStart();

	UFUNCTION(BlueprintCallable)
	void AnimNotify_NOT_ResetLadder();


	void SetLadderIK(const FName& BoneName, const FName& MiddleBoneName, float CurveValue, FVector& BoneTarget, float& LimbLadderAlpha, float DeltaSeconds, float Offset = 1.0f);

	void CheckIKValid(FName CurveName, float& AlphaValue, float DeltaSeconds);
	TOptional<FVector> SetBodyLocationOnLadder(FName BoneName, FName MiddleBoneName, float CurveValue, FVector PrevTargetLoc, float DeltaSeconds, float AdjCoefft = 1.0f);
	
private:
	bool bIsClimb;

#pragma endregion 

#pragma endregion 


#pragma region Character IK
private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ladder, Meta = (AllowPrivateAccess = true))
		float LadderIKAlpha;

	FName LockIK = TEXT("LockIK");
	TMap<FName, EBodyType> BoneNameToBodyType;

#pragma region FootIK
////////////////////////////////////
// Variables For FootIK
////////////////////////////////////
private:
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

	// For Ladder IK
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FootIK, Meta = (AllowPrivateAccess = true))
		float RightFootLadderAlpha;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FootIK, Meta = (AllowPrivateAccess = true))
		float LeftFootLadderAlpha;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FootIK, Meta = (AllowPrivateAccess = true))
		float RightFootStateAlpha;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FootIK, Meta = (AllowPrivateAccess = true))
		float LeftFootStateAlpha;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FootIK, Meta = (AllowPrivateAccess = true))
		FVector LeftFootTarget;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FootIK, Meta = (AllowPrivateAccess = true))
		FVector RightFootTarget;

	FName Foot_L_Curve = TEXT("Enable_Footik_l");
	FName Foot_R_Curve = TEXT("Enable_FootIK_R");
	FName Pelvis_Curve = TEXT("Enable_Pelvis");

	float CurveValue_Foot_L;
	float CurveValue_Foot_R;
	FVector CurveValue_Pelvis;
	FRotator CurveValue_Pelvis_Rotator;


#pragma endregion 

#pragma region HandIK

////////////////////////////////////
// Methods For HandIK
////////////////////////////////////

////////////////////////////////////
// Variables For HandIK
////////////////////////////////////
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HandIK, Meta = (AllowPrivateAccess = true))
		float RightHandStateAlpha;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HandIK, Meta = (AllowPrivateAccess = true))
		float LeftHandStateAlpha;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HandIK, Meta = (AllowPrivateAccess = true))
		float LeftHandLadderAlpha;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HandIK, Meta = (AllowPrivateAccess = true))
		float RightHandLadderAlpha;

	// Actual Hand Position
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HandIK, Meta = (AllowPrivateAccess = true))
		FVector LeftHandTarget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HandIK, Meta = (AllowPrivateAccess = true))
		FVector RightHandTarget;


	// For Ladder IK
	FName Hand_L_Curve = TEXT("Enable_HandIK_L");
	FName Hand_R_Curve = TEXT("Enable_HandIK_R");

	float CurveValue_Hand_L;
	float CurveValue_Hand_R;

#pragma endregion
	

#pragma endregion 



#pragma region Debug Region
private:
	void DebugLadderStance();

private:
	float PelvisCurveSum = 0.0f;
	float AdjustCurveSum = 0.0f;
#pragma endregion
};