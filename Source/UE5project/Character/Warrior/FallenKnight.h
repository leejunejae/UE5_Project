 
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../CharacterBase.h"
#include "Components/TimeLineComponent.h"
#include "FallenKnight.generated.h"

/**
 * 
 */

class UCharacterBaseAnimInstance;

UCLASS()
class UE5PROJECT_API AFallenKnight : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	AFallenKnight();

	virtual void BeginPlay() override;
	bool IsAttackInput();
	bool IsAttacking();
	bool IsRolling();
	bool GetNextDodge();
	int32 CheckCombo();
	WarriorStance GetStance();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

private:
	UPROPERTY(VisibleAnywhere, Category = Equipment)
		USkeletalMeshComponent* TopMesh;
	UPROPERTY(VisibleAnywhere, Category = Equipment)
		USkeletalMeshComponent* BottomMesh;
	UPROPERTY(VisibleAnywhere, Category = Equipment)
		USkeletalMeshComponent* ShoesMesh;
	UPROPERTY(VisibleAnywhere, Category = Equipment)
		USkeletalMeshComponent* GlovesMesh;
	UPROPERTY(VisibleAnywhere, Category = Equipment)
		USkeletalMeshComponent* HatMesh;

	UPROPERTY(VisibleAnywhere, Category = Equipment)
		UStaticMeshComponent* SheatheMesh;

	/*
	UPROPERTY(VisibleAnywhere, Category = Equipment)
		USkeletalMeshComponent* BracerMesh;
	UPROPERTY(VisibleAnywhere, Category = Equipment)
		USkeletalMeshComponent* CapeMesh;
	UPROPERTY(VisibleAnywhere, Category = Equipment)
		USkeletalMeshComponent* ShoulderMesh;
	UPROPERTY(VisibleAnywhere, Category = Equipment)
		USkeletalMeshComponent* GorgetMesh;
	UPROPERTY(VisibleAnywhere, Category = Equipment)
		USkeletalMeshComponent* ArmorMesh;
		*/

	FTimerHandle MoveSpeedTimerHandle;

	//Dodge TImeline
	UPROPERTY(EditAnywhere, Category = Timeline)
		UCurveFloat* DodgeCurve;

	FVector InitPosition;
	FTimeline DodgeTimeline;
	float DodgeTimelineLength;
	float PreviousCurveValue;

	UFUNCTION()
		void DodgeUpdate(float Value);

	UFUNCTION()
		void DodgeUpdateFin();

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool CanNextCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 CurrentCombo=0;

	bool IsInvincible;
	bool AttackInput;
	bool CanAttack;
	bool NextDodge;
	bool CanInputBuffer;

	bool SetCharDirection;
	FRotator TargetRotation;

	WarriorStance CurStance = WarriorStance::SNS;

private:
	void MoveSpeedTimer();

	virtual void Sprint() override;
	virtual void Attack() override;

	void AttackInputEnd();
	void Dodge() override;

	void SwitchStance() override;
	void ResetAttackState();
	void AttackTimer();

	FTimerHandle AttackTimerHandle;

	UFUNCTION()
		void IsMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
		void IsMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);
};