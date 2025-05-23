// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../PEnumHeader.h"
#include "../../BaseCharacterHeader.h"
#include "../../Function/Combat/CombatStruct.h"
#include "../../Function/IAnimInstance.h"
#include "Animation/AnimInstance.h"
#include "CharacterBaseAnimInstance.generated.h"

DECLARE_DELEGATE(FOnSingleDelegate);
DECLARE_MULTICAST_DELEGATE(FOnMultiDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMulOneParamDelegate, FName);
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
struct FVectorCurveNameSet
{
	FName CurveNameX;
	FName CurveNameY;
	FName CurveNameZ;
};

UCLASS()
class UE5PROJECT_API UCharacterBaseAnimInstance : public UAnimInstance, public IIAnimInstance
{
	GENERATED_BODY()

public:
	UCharacterBaseAnimInstance();
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	FOnSingleDelegate OnEnterLocomotion;
	FOnSingleDelegate OnLeftLocomotion;

	FOnMultiDelegate OnResetHurt;
	FOnMultiDelegate OnSetAttackDirection;
	FOnMultiDelegate OnAttackStart;
	FOnMultiDelegate OnCanDodge;

	FOnMultiDelegate OnParryEnd;
	FOnMultiDelegate OnNextAttackCheck;
	FOnMultiDelegate OnEndAttack;
	FOnMultiDelegate OnMountEnd;
	FOnMultiDelegate OnDisMountEnd;

	FOnMultiDelegate OnDodgeEnd;
	FOnMultiDelegate OnDodgeStart;
	FOnMultiDelegate OnEquipEnd;
	FOnMultiDelegate OnHolsterEnd;

// Ladder Delegate
	FOnMultiDelegate OnClimbEnd;
	FOnMulOneParamDelegate OnNextGrip;

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

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Character, Meta = (AllowPrivateAccess = true))
		bool CombatMode;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Character, Meta = (AllowPrivateAccess = true))
		bool IsTurning=false;

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
		float LocomotionAnimTime;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stance, Meta = (AllowPrivateAccess = true))
		ERideStance CurRideStance;

public:
	FOnMultiDelegate OnEnterWalkState;
	FOnMultiDelegate OnEnterLadderState;

#pragma region Ground
private:
	UFUNCTION(BlueprintCallable)
		void AnimNotify_NOT_EnableRootLock();
	UFUNCTION(BlueprintCallable)
		void AnimNotify_NOT_DisableRootLock();

protected:
	// Foot IK //
	TTuple<FVector, float> FootTrace(FName SocketName);
	void FootRotation(float DeltaTime, FVector TargetNormal, FRotator* FootRotator, float fInterpSpeed);

#pragma region Movement
	////////////////////////////////////
	// Variables For FootIK
	////////////////////////////////////
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Movement, Meta = (AllowPrivateAccess = true))
		bool CanMovementInput = true;

	float CharacterYaw;

	float GetAnimDirection(float DeltaSeconds);

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


	void SetLadderIK(const FName& BoneName, const FName& MiddleBoneName, FVectorCurveNameSet CurveNameList, FVector& BoneTarget, float LimbYDistance, float DeltaSeconds, float Offset = 1.0f, bool IsDebug = false);
	
	void CheckIKValid(FName CurveName, float& AlphaValue, float DeltaSeconds);
	TOptional<FVector> SetIKTargetLocation(FName BoneName, FName MiddleBoneName, float CurveValue, float DeltaSeconds, float AdjCoefft = 1.0f);

private:
	bool bIsClimb;
	float CurveValue_Root_Z;
	float CurveValue_Root_Y;
	float CurveValue_Root_Rotator;

	float RootCurveDifferenceSum = 0.0f;
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

	// For Ladder IK
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FootIK, Meta = (AllowPrivateAccess = true))
		float RightFootIKAlpha;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FootIK, Meta = (AllowPrivateAccess = true))
		float LeftFootIKAlpha;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FootIK, Meta = (AllowPrivateAccess = true))
		float RightFootIKTypeAlpha;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FootIK, Meta = (AllowPrivateAccess = true))
		float LeftFootIKTypeAlpha;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FootIK, Meta = (AllowPrivateAccess = true))
		FVector LeftFootTarget;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FootIK, Meta = (AllowPrivateAccess = true))
		FVector RightFootTarget;

	FVectorCurveNameSet Hand_L_CurveNameSet = { TEXT("Hand_L_Translation_X"), TEXT("Hand_L_Translation_Y") , TEXT("Hand_L_Translation_Z") };
	FVectorCurveNameSet Hand_R_CurveNameSet = { TEXT("Hand_R_Translation_X"), TEXT("Hand_R_Translation_Y") , TEXT("Hand_R_Translation_Z") };
	FVectorCurveNameSet Foot_L_CurveNameSet = { TEXT("Foot_L_Translation_X"), TEXT("Foot_L_Translation_Y") , TEXT("Foot_L_Translation_Z") };
	FVectorCurveNameSet Foot_R_CurveNameSet = { TEXT("Foot_R_Translation_X"), TEXT("Foot_R_Translation_Y") , TEXT("Foot_R_Translation_Z") };

	float Hand_L_Y_Distance;
	float Hand_R_Y_Distance;
	float Foot_L_Y_Distance;
	float Foot_R_Y_Distance;

	FName Foot_L_Translation_Z = TEXT("Foot_L_Translation_Z");
	FName Foot_R_Translation_Z = TEXT("Foot_R_Translation_Z");
	FName Foot_L_Translation_Y = TEXT("Foot_L_Translation_Y");
	FName Foot_R_Translation_Y = TEXT("Foot_R_Translation_Y");


	FName Pelvis_Curve = TEXT("Enable_Pelvis");
	

	//FName Foot_L_Translation_Z = TEXT("Enable_Footik_l");
	//FName Foot_R_Translation_Z = TEXT("Enable_FootIK_R");
	//FName Pelvis_Curve = TEXT("Enable_Pelvis");

	FVector CurveValue_Foot_L;
	FVector CurveValue_Foot_R;


#pragma endregion 

#pragma region HandIK

////////////////////////////////////
// Methods For HandIK
////////////////////////////////////Foot_L_Translation_Z

////////////////////////////////////
// Variables For HandIK
////////////////////////////////////
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HandIK, Meta = (AllowPrivateAccess = true))
		float RightHandIKTypeAlpha;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HandIK, Meta = (AllowPrivateAccess = true))
		float LeftHandIKTypeAlpha;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HandIK, Meta = (AllowPrivateAccess = true))
		float LeftHandIKAlpha;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HandIK, Meta = (AllowPrivateAccess = true))
		float RightHandIKAlpha;

	// Actual Hand Position
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HandIK, Meta = (AllowPrivateAccess = true))
		FVector LeftHandTarget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HandIK, Meta = (AllowPrivateAccess = true))
		FVector RightHandTarget;

	FName Hand_L_Translation_Z = TEXT("Hand_L_Translation_Z");
	FName Hand_R_Translation_Z = TEXT("Hand_R_Translation_Z");

	FName Hand_L_Translation_Y = TEXT("Hand_L_Translation_Y");
	FName Hand_R_Translation_Y = TEXT("Hand_R_Translation_Y");

	// For Ladder IK
	//FName Hand_L_Translation_Z = TEXT("Enable_HandIK_L");
	//FName Hand_R_Translation_Z = TEXT("Enable_HandIK_R");

	FVector CurveValue_Hand_L;
	FVector CurveValue_Hand_R;

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