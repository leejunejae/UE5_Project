// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemies/Human/PBEHAnimInstance.h"
#include "Components/TimeLineComponent.h"
#include "PBEHHSAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnMultDelegate);
/**
 * 
 */
UCLASS()
class UE5PROJECT_API UPBEHHSAnimInstance : public UPBEHAnimInstance
{
	GENERATED_BODY()
	
public:
	UPBEHHSAnimInstance();
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	void PlayMontage(FName MontageName) override;
	MontageType CheckMontage(UAnimMontage* Montage) override;

public:
	FOnMultDelegate OnNextCombo;
	FOnMultDelegate OnEndCombo;
	FOnMultDelegate OnHitEnd;
	FOnMultDelegate OnHitLargeEnd;
	FOnMultDelegate OnBlockEnd;
	FOnMultDelegate OnBlockBreakEnd;
	FOnMultDelegate OnStartAttack;
	FOnMultDelegate OnEndAttack;

	FOnMultDelegate OnDashEnd;
	FOnMultDelegate OnEnterLocomotion;
	FOnMultDelegate OnLeftLocomotion;

private:
	class APBEHHereticSoldier* HSoldier = nullptr;

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* Combo1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		bool IsAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		bool IsHit;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		bool IsHitLarge;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		bool IsBlock;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		bool IsDash;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		bool IsDefenseMode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		bool IsBlockBreak;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		bool IsParried;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		int32 ComboCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		float InputDirection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		HSoldierSkill CurSkill = HSoldierSkill::None;


	float InputDirectionX;
	float InputDirectionY;

	//Turn In Place && Focus

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TIP, Meta = (AllowPrivateAccess = true))
		float UpdateAngle;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TIP, Meta = (AllowPrivateAccess = true))
		float AimAlpha;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TIP, Meta = (AllowPrivateAccess = true))
		bool IsTurning;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		bool IsLookAt;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		bool CanLookAt = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		FVector LookAtLocation;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TIP, Meta = (AllowPrivateAccess = true))
		FRotator AngleFromTarget;

	AActor* LookAtTarget;
	FVector LastFrameLookAtLocation;

private:
	UFUNCTION()
		void AnimNotify_NOT_CanNextCombo();
	UFUNCTION()
		void AnimNotify_NOT_EndCombo();
	UFUNCTION()
		void AnimNotify_NOT_StartAttack();
	UFUNCTION()
		void AnimNotify_NOT_EndAttack();
	UFUNCTION()
		void AnimNotify_NOT_HitEnd();
	UFUNCTION()
		void AnimNotify_NOT_HitLargeEnd();
	UFUNCTION()
		void AnimNotify_NOT_BlockBreakEnd();
	UFUNCTION(BlueprintCallable)
		void AnimNotify_NOT_DashEnd();
	UFUNCTION(BlueprintCallable)
		void AnimNotify_NOT_TurnEnd();
	UFUNCTION(BlueprintCallable)
		void AnimNotify_NOT_EnterLocomotion();
	UFUNCTION(BlueprintCallable)
		void AnimNotify_NOT_LeftLocomotion();
};
