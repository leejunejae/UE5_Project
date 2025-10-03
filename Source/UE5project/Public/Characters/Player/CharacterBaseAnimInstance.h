// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// 엔진 헤더
#include "CoreMinimal.h"

// 구조체, 자료형
#include "PEnumHeader.h"
#include "Characters/Data/BaseCharacterHeader.h"
#include "Combat/Data/CombatData.h"
#include "Characters/Data/StatusData.h"
#include "Interaction/Climb/Data/ClimbHeader.h"
#include "Characters/Data/IKData.h"
#include "Animation/AnimData.h"
#include "Items/Weapons/Data/WeaponData.h"

// 인터페이스
#include "Animation/Interfaces/IAnimInstance.h"

// 애님모드
#include "Animation/AnimModeBase.h"

#include "Animation/AnimInstance.h"
#include "CharacterBaseAnimInstance.generated.h"

DECLARE_DELEGATE(FOnSingleDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAnimInstanceMulDel);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMulOneParamDelegate, FName);

UCLASS()
class UE5PROJECT_API UCharacterBaseAnimInstance : public UAnimInstance, public IIAnimInstance
{
	GENERATED_BODY()

	friend class UAnimModeBase;
	friend class UAnimMode_Ground;
	friend class UAnimMode_Ladder;
	friend class UAnimMode_Ride;

public:
	UCharacterBaseAnimInstance();
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	class APlayerBase* Character = nullptr;

#pragma region Animation Mode
protected:
	UPROPERTY(VisibleAnywhere)
		TMap<ECharacterState, UAnimModeBase*> AnimModeMap;
	
	UPROPERTY(VisibleAnywhere)
		TObjectPtr<UAnimModeBase> CurrentMode = nullptr;

	void SwitchAnimMode(const ECharacterState TargetMode);

#pragma endregion Animation Mode


#pragma region Animation Data
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Anim, Meta = (AllowPrivateAccess = true)) 
		TObjectPtr<UBlendSpace1D> Locomotion_CycleBS = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Anim, Meta = (AllowPrivateAccess = true))
		TObjectPtr<UAnimSequence> Locomotion_Idle = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Anim, Meta = (AllowPrivateAccess = true))
		TObjectPtr<UAnimSequence> Locomotion_Start = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Anim, Meta = (AllowPrivateAccess = true))
		TObjectPtr<UAnimSequence> Locomotion_Stop_Jog = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Anim, Meta = (AllowPrivateAccess = true))
		TObjectPtr<UAnimSequence> Locomotion_Stop_Run = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Anim, Meta = (AllowPrivateAccess = true))
		TObjectPtr<UAnimSequence> Jump_Start = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Anim, Meta = (AllowPrivateAccess = true))
		TObjectPtr<UAnimSequence> Jump_Loop = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Anim, Meta = (AllowPrivateAccess = true))
		TObjectPtr<UAnimSequence> Fall_Loop = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Anim, Meta = (AllowPrivateAccess = true))
		TObjectPtr<UAnimSequence> Land_Jump = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Anim, Meta = (AllowPrivateAccess = true))
		TObjectPtr<UAnimSequence> Land_Fall = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Anim, Meta = (AllowPrivateAccess = true))
		TObjectPtr<UAnimSequence> Land_Jog = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Anim, Meta = (AllowPrivateAccess = true))
		TObjectPtr<UAnimSequence> Land_High = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Anim, Meta = (AllowPrivateAccess = true))
		TObjectPtr<UAnimSequence> HitAir_Start = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Anim, Meta = (AllowPrivateAccess = true))
		TObjectPtr<UAnimSequence> HitAir_Loop = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Anim, Meta = (AllowPrivateAccess = true))
		TObjectPtr<UAnimSequence> HitAir_End = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Anim, Meta = (AllowPrivateAccess = true))
		TObjectPtr<UAnimSequence> GetUp = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Anim, Meta = (AllowPrivateAccess = true))
		TObjectPtr<UAnimSequence> HitAir_Death = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Anim, Meta = (AllowPrivateAccess = true))
		TObjectPtr<UAnimSequence> Ground_Death = nullptr;

	void HandleWeaponChange(EWeaponType WeaponData);

	bool bInitAnimSet = false;
#pragma endregion Animation Data

#pragma region State & Stance
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, Meta = (AllowPrivateAccess = true))
		EGroundStance CharacterGroundState;

#pragma region State & Stance_IK
public:
	virtual void SetIKWeight_Implementation(EIKContext IKContext, ELimbList Limb, float Weight);
	virtual float GetIKWeight_Implementation(EIKContext IKContext, ELimbList Limb);
#pragma endregion State & Stance_IK

private:
//	UFUNCTION(BlueprintCallable)
		//void AnimNotify_NOT_EnterWalkState();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
		ECharacterState CurrentState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stance, Meta = (AllowPrivateAccess = true))
		EClimbPhase CurLadderStance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stance, Meta = (AllowPrivateAccess = true))
		ERideStance CurRideStance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = IK, Meta = (AllowPrivateAccess = true))
		FIKContextWeights GroundIKWeights;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = IK, Meta = (AllowPrivateAccess = true))
		FIKContextWeights LadderIKWeights;

public:
	FOnAnimInstanceMulDel OnEnterWalkState;
	FOnAnimInstanceMulDel OnEnterLadderState;

#pragma region Ground
private:
	UFUNCTION(BlueprintCallable)
		void AnimNotify_NOT_EnableRootLock();
	UFUNCTION(BlueprintCallable)
		void AnimNotify_NOT_DisableRootLock();

protected:
	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "Ground|Locomotion", meta = (AllowPrivateAccess = "true"))
		bool IsMovementInput;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "Ground|Locomotion", meta = (AllowPrivateAccess = "true"))
		float Speed;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "Ground|Locomotion", meta = (AllowPrivateAccess = "true"))
		float Direction;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "Ground|Locomotion", meta = (AllowPrivateAccess = "true"))
		float LocomotionAnimTime;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "Ground|Locomotion", meta = (AllowPrivateAccess = "true"))
		float LastDirection;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "Ground|Locomotion", meta = (AllowPrivateAccess = "true"))
		float LastSpeed;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "Ground|Locomotion", meta = (AllowPrivateAccess = "true"))
		bool IsAccelerating;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "Ground|Jump", meta = (AllowPrivateAccess = "true"))
		bool IsInAir;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "Ground|Jump", meta = (AllowPrivateAccess = "true"))
		bool IsJumping;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "Ground|Jump", meta = (AllowPrivateAccess = "true"))
		bool IsFalling;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "Ground|Jump", meta = (AllowPrivateAccess = "true"))
		bool IsLanding;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "Ground|Combat", meta = (AllowPrivateAccess = "true"))
		float BlockBlend;

	// Foot IK //
	TTuple<FVector, float> FootTrace(FName SocketName);
	void FootRotation(float DeltaTime, FVector TargetNormal, FRotator* FootRotator, float fInterpSpeed);

#pragma region Ground_IK
protected:
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = FootIK, Meta = (AllowPrivateAccess = true))
		FRotator LeftFootRotator;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = FootIK, Meta = (AllowPrivateAccess = true))
		FRotator RightFootRotator;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = FootIK, Meta = (AllowPrivateAccess = true))
		float PelvisOffset;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = FootIK, Meta = (AllowPrivateAccess = true))
		FVector LeftFootGroundOffset;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = FootIK, Meta = (AllowPrivateAccess = true))
		FVector RightFootGroundOffset;

#pragma endregion Ground_IK

#pragma region Ground_Movement
	////////////////////////////////////
	// Variables For FootIK
	////////////////////////////////////
private:
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Movement, Meta = (AllowPrivateAccess = true))
		float MovementAlpha;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Movement, Meta = (AllowPrivateAccess = true))
		EAnimDirection CurrentDirection;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Movement, Meta = (AllowPrivateAccess = true))
		bool CanMovementInput = true;

	//float CharacterYaw;
	//float GetAnimDirection(float DeltaSeconds);

	//bool IsFirstUpdateYaw;

#pragma endregion Ground_Movement

#pragma endregion Ground


#pragma region Ladder

#pragma region Ladder_IK
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FootIK, Meta = (AllowPrivateAccess = true))
		FVector LeftFootLadderOffset;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FootIK, Meta = (AllowPrivateAccess = true))
		FVector RightFootLadderOffset;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HandIK, Meta = (AllowPrivateAccess = true))
		FVector LeftHandLadderOffset;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HandIK, Meta = (AllowPrivateAccess = true))
		FVector RightHandLadderOffset;
#pragma endregion Ladder_IK

protected:
	UFUNCTION(BlueprintCallable)
	void AnimNotify_NOT_ResetClimbState();
#pragma endregion Ladder

#pragma endregion State & Stance


#pragma region HitReaction
public:
	void SetHitAir(bool HitState);
	void ResetHitAir_Implementation() override;

protected:
	FORCEINLINE void HandleDeath() { bIsDead = true; }

private:
	UPROPERTY(BlueprintReadOnly, Category = HitReaction, Meta = (AllowPrivateAccess = true))
		bool bIsHitAir = false;

	UPROPERTY(BlueprintReadOnly, Category = HitReaction, Meta = (AllowPrivateAccess = true))
		bool bIsDead = false;

#pragma endregion HitReaction
};