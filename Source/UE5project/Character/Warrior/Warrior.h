// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../CharacterBase.h"
#include "Components/TimeLineComponent.h"
#include "Warrior.generated.h"

/**
 * 
 */

class UPCWAnimInstance;

UCLASS()
class UE5PROJECT_API AWarrior : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	AWarrior();

	virtual void BeginPlay() override;

	virtual void Death() override;

	virtual void Block(bool CanParried) override;


	/* Function for Hit */
	void TakeDamage_Implementation(FAttackInfo DamageInfo) override;
	HitResponse CharResponse(bool CanBlocked, bool CanAvoid, bool CanParry);
	CharState GetCharState();
	HitResponse GetCharResponse();
	void ResetResponse();

	int GetAttackSeed();
	bool IsAttackInput();
	bool IsAttacking();
	bool IsRolling();
	bool IsBlocking();
	bool IsParrying();
	bool IsHit();
	bool IsGuard();
	bool GetCurHand();
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
		USkeletalMeshComponent* BracerMesh;
	UPROPERTY(VisibleAnywhere, Category = Equipment)
		USkeletalMeshComponent* CapeMesh;
	UPROPERTY(VisibleAnywhere, Category = Equipment)
		USkeletalMeshComponent* ShoulderMesh;
	UPROPERTY(VisibleAnywhere, Category = Equipment)
		USkeletalMeshComponent* GorgetMesh;
	UPROPERTY(VisibleAnywhere, Category = Equipment)
		USkeletalMeshComponent* ArmorMesh;

	UPROPERTY(VisibleAnywhere, Category = Input)
		UInputAction* ParryAction;

	FTimerHandle MoveSpeedTimerHandle;

	UPROPERTY(VisibleAnywhere, Category = Animation)
		class UPCWAnimInstance* WarriorAnim;

	UPROPERTY(VisibleAnywhere, Category = Combat)
		class UPBDamageSystem* WarriorDMGSystem;

	//Dodge TImeline
	UPROPERTY(VisibleAnywhere, Category = Timeline)
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

	int32 AttackSeed;

	bool Hit;
	bool Guard;
	HitResponse CurResponse;

	bool IsInvincible;
	bool IsParry;
	bool AttackInput;
	bool CanAttack;
	bool CanDodge;
	bool NextDodge;
	bool CurHandRight;
	bool CanInputBuffer;

	bool SetCharDirection;
	FRotator TargetRotation;

	WarriorStance CurStance = WarriorStance::SNS;

private:
	void MoveSpeedTimer();

	virtual void MoveSpeedToggle() override;
	virtual void Attack() override;

	void AttackInputEnd();
	void Dodge() override;
	void Parrying();

	void SwitchStance() override;
	void ResetAttackState();
	void AttackTimer();
	void OnBlock();
	void OffBlock();

	void SetAttackInfo(float Amount, HitResponse Response, bool Invincible = false, bool CanBlocked = false, bool CanParried = false, bool ForceInterrupt = true);

	FTimerHandle AttackTimerHandle;

	TQueue<WarriorVar> ActionQueue;

	UFUNCTION()
		void IsMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
		void IsMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		TArray<USoundCue*> RollSounds;
};