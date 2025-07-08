// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// 기본 헤더
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"

// 구조체, 자료형 
#include "Characters/Data/BaseCharacterHeader.h"
#include "Interaction/Climb/Data/ClimbHeader.h"
#include "PEnumHeader.h"
#include "Characters/Data/StatusData.h"
// 인터페이스
#include "Characters/Player/Interfaces/PlayerInterface.h"
#include "Combat/Interfaces/HitReactionInterface.h"
#include "Interaction/Climb/Interfaces/ClimbInterface.h"
#include "Characters/Player/Interfaces/ViewDataInterface.h"
#include "Characters/Interfaces/CharacterTransformInterface.h"

// 컴포넌트
#include "CharacterBase.generated.h"


class UInputMappingContext;
class UInputAction;
class UCameraComponent;
class USpringArmComponent;
class UCharacterMovementComponent;

class UCharacterStatusComponent;
class UStatComponent;
class UEquipmentComponent;
class UCombatComponent;
class UPlayerAttackComponent;
class UHitReactionComponent;
class UClimbComponent;
class UInteractComponent;

class UDefaultWidget;

//class USkeletalMeshComponent;
//class UStaticMeshComponent;;

// Delegates
DECLARE_DELEGATE(FOnSingleDelegate);

UCLASS()
class UE5PROJECT_API ACharacterBase : public ACharacter, 
	public IHitReactionInterface, 
	public IPlayerInterface, 
	public IClimbInterface, 
	public IViewDataInterface,
	public ICharacterTransformInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

	
/* PRIVATE VARIATION */
private:
	UPROPERTY(EditAnywhere)
		TSubclassOf<UDefaultWidget> DefaultWidgetClass;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UDefaultWidget> DefaultWidget;


	float YAxisScale;
	float DebugUpdateInterval = 0.1f; // 디버깅 갱신 간격
	float TimeSinceLastDebugUpdate = 0.0f;
	FVector PastLastInputVector;
/* PRIVATE VARIATION */


/* PROTECTED VARIATION */
protected:
	MovementDirection CurrentDirection;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		TObjectPtr<UCameraComponent> Camera;


	/* ĳ���� �Է� ���� ���� */
	UPROPERTY(EditAnywhere, Category = Input)
		TObjectPtr<UInputMappingContext> DefaultContext;

	UPROPERTY(EditAnywhere, Category = Input)
		TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = Input)
		TObjectPtr<UInputAction> CheckMoveAction;

	UPROPERTY(EditAnywhere, Category = Input)
		TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = Input)
		TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = Input)
		TObjectPtr<UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, Category = Input)
		TObjectPtr<UInputAction> HeavyAttackAction;

	UPROPERTY(EditAnywhere, Category = Input)
		TObjectPtr<UInputAction> DodgeAction;

	UPROPERTY(EditAnywhere, Category = Input)
		TObjectPtr<UInputAction> BlockAction;

	UPROPERTY(EditAnywhere, Category = Input)
		TObjectPtr<UInputAction> ParryAction;

	UPROPERTY(EditAnywhere, Category = Input)
		TObjectPtr<UInputAction> WalkAction;

	UPROPERTY(EditAnywhere, Category = Input)
		TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(EditAnywhere, Category = Input)
		TObjectPtr<UInputAction> SwitchStanceAction;

	UPROPERTY(EditAnywhere, Category = Input)
		TObjectPtr<UInputAction> InteractAction;

	UPROPERTY(EditAnywhere, Category = Input)
		TObjectPtr<UInputAction> SpawnRideAction;
	
	UPROPERTY(EditAnywhere, Category = Input)
		TObjectPtr<UInputAction> ModifierAction;

	FVector2D AimOffVal;

	bool IsLocomotion = true;
	bool IsMovementInput;
	bool IsClimb;
	bool IsClimbUp;

	FVector InputVector;
	FVector DodgeVector;
	FVector DodgeDirection;

	bool IsAction;
	bool IsRun;
	bool IsAttack;
	bool IsRoll;
	bool CanDodge;
	bool IsInteraction;
	bool CanInput=true;
	float InputDirection;
	float TargetSpeed=300.0f;

	ACharacter* Ride;

	FVector InitSpringArmLocation;
/* PROTECTED VARIATION */

/* PROTECTED FUNCTION */
protected:

	/* ĳ������ �⺻���� �������� �����ϴ� �Լ�*/
	void Move(const FInputActionValue& value);
	void Look(const FInputActionValue& value);

	void StartMoveInput();
	void EndMoveInput();

	virtual void Dodge();
	virtual void SwitchStance();
	void CameraSetting();
	
	void Jump();
	void Landed(const FHitResult& Hit) override;

	virtual void Interact();

/* PROTECTED FUNCTION */

/* Public VARIATION */
public:

/* Public VARIATION */

/* Public FUNCTION */
public:
	virtual bool IsPlayer_Implementation();
	virtual TOptional<FVector> GetCharBoneLocation(FName BoneName);

	bool GetIsMovementInput();
	float GetRideSpeed();
	float GetRideDirection();
	FVector GetInputDirection();


/* Public FUNCTION */
#pragma region Input
private:
	FORCEINLINE void ModifierInput() { IsModifierInput = true; }
	FORCEINLINE void ModifierInputEnd() { IsModifierInput = false; }

	bool IsModifierInput = false;
#pragma region Input

#pragma region Status
private:
	UPROPERTY(VisibleAnywhere, Category = Stat)
		TObjectPtr<UStatComponent> StatComponent;

	UPROPERTY(VisibleAnywhere, Category = Status)
		TObjectPtr<UCharacterStatusComponent> CharacterStatusComponent;

	bool IsBlockInput = false;

public:
	FORCEINLINE UStatComponent* GetStatComponent() const { return StatComponent; }
	FORCEINLINE UCharacterStatusComponent* GetCharacterStatusComponent() const { return CharacterStatusComponent; }


#pragma endregion Status

#pragma region Inventory & Equip
private:
	UPROPERTY(VisibleAnywhere, Category = Equip)
		TObjectPtr<UEquipmentComponent> EquipmentComponent;

public:
	FORCEINLINE UEquipmentComponent* GetEquipmentComponent() const { return EquipmentComponent; }

#pragma endregion Inventory & Equip

#pragma region Animation
protected:
	UPROPERTY(VisibleAnywhere, Category = Animation)
		class UCharacterBaseAnimInstance* CharacterBaseAnim;

#pragma endregion Animation


#pragma region State & Stance
// 'State' is the character's situation to distinguish the character's actions.
// 	it usually mean where the character is, like on riding, on ladder, on ground...

// 'Stance' is the specific behavior of a character within a state.
//	if character is on a ladder, 'Stance' indicates what the character is doing on the ladder, like climbup, just hanging...


////////////////////////////////////
// Variables For State & Stance
////////////////////////////////////	
protected:
	ECharacterState CurrentState = ECharacterState::Ground;
	ELadderStance CurLadderStance = ELadderStance::Idle;
	EGroundStance CurGroundStance = EGroundStance::Walk;
	ERideStance CurRideStance = ERideStance::Riding;


////////////////////////////////////
// Methods For State & Stance
////////////////////////////////////
public:
	ECharacterState GetCurrentState();
	ELadderStance GetCurLadderStance();
	EGroundStance GetCurGroundStance();
	ERideStance GetCurRideStance();
	float GetClimbDistance();
#pragma endregion State & Stance

#pragma region Ground
protected:
	virtual void Walk();
	virtual void WalkEnd();
	virtual void Sprint();
	virtual void SprintEnd();
	void EnterLocomotion();
	void LeftLocomotion();

public:
	float GetDirection();

	void SetRotationInputDirection_Implementation();

private:
	float Direction;

	FRotator InputRotation;
	bool bForcedRotatingInputDirection = false;
	float ForcedRotationSpeed = 720.0f;

#pragma endregion Ground

#pragma region Ladder
private:
	FGripNode1D* Grip1D_Hand_R;
	FGripNode1D* Grip1D_Hand_L;
	FGripNode1D* Grip1D_Foot_R;
	FGripNode1D* Grip1D_Foot_L;
	float ClimbDistance;
	bool CanMovementInput;

protected:
	void DecideLadderStance();

	UPROPERTY(VisibleAnywhere, Category = Climb)
		UClimbComponent* ClimbComponent;

	UPROPERTY(EditAnywhere)
		float MinGripInterval = 15.0f;
	UPROPERTY(EditAnywhere)
		float MaxGripInterval = 50.0f;
	UPROPERTY(EditAnywhere)
		float MinFirstGripHeight = 40.0f;

public:
	FORCEINLINE UClimbComponent* GetClimbComponent() const { return ClimbComponent; }
	void SetCanMovementInput(bool CanMove);
	virtual void SetNextGripDown_Implementation(FName BoneName, int32 Count) override;

#pragma endregion


#pragma region Character Bone

private:
	EBodyType CharBone;

public:
	TOptional<TTuple<FVector, FVector>> GetLadderIKTargetLoc(EBodyType BoneType);
	TOptional<FVector> GetRideIKTargetLoc(EBodyType BoneType);
#pragma endregion

#pragma region Ride
private:
	void MountTimer();
	
	FTimerHandle MountTimerHandle;

	void SpawnRide();
	void DespawnRide_Implementation(FVector InitVelocity);

	void CameraSettingTimer();
	FTimerHandle CameraSettingTimerHandle;

	void JumpDismountTimer();
	FTimerHandle JumpDismountTimerHandle;

protected:
	void MountEnd();
	void DisMountEnd();

public:
	virtual FTransform GetCameraTransform_Implementation();
	virtual FTransform GetSpringArmTransform_Implementation();
	virtual float GetTargetArmLength_Implementation();
	virtual FRotator GetControllerRotation_Implementation();


private:
	bool CanRide;
#pragma endregion


#pragma region Interact
protected:
	void HandleArrivedInteractionPoint();

	UPROPERTY(VisibleAnywhere, Category = Interact)
		UInteractComponent* InteractComponent;

public:
	virtual void RegisterInteractableActor_Implementation(AActor* Interactable);
	virtual void DeRegisterInteractableActor_Implementation(AActor* Interactable);
	virtual void EndInteraction_Implementation(AActor* Interactable);

	FORCEINLINE UInteractComponent* GetInteractComponent() const { return InteractComponent; }
#pragma endregion 

#pragma region Combat
private:
	UPROPERTY(VisibleAnywhere, Category = Combat)
		UCombatComponent* CombatComponent;

	UPROPERTY(VisibleAnywhere, Category = Attack)
		UPlayerAttackComponent* AttackComponent;

	virtual void Attack(FName AttackName);
	void AttackInput();
	void AttackInputEnd();

	bool IsAttackInput;

	void ChargeAttackTimer();
	FTimerHandle AttackTimerHandle;
	float AttackChargeTime = 0.0f;

public:
	FORCEINLINE UCombatComponent* GetCombatComponent() const { return CombatComponent; }
	FORCEINLINE UPlayerAttackComponent* GetAttackComponent() const { return AttackComponent; }

#pragma region HitReaction
protected:
	UPROPERTY(VisibleAnywhere, Category = HitReaction)
		UHitReactionComponent* HitReactionComponent;

	void OnBlock();
	void OffBlock();

	void Parry();

	void HandleHitAir();

public:
	virtual void OnHit_Implementation(const FAttackRequest& AttackInfo) override;
	virtual void OnDeathEnd_Implementation() override;
	virtual void OnDeath();

	FORCEINLINE UHitReactionComponent* GetHitReactionComponent() const { return HitReactionComponent; }
#pragma endregion HitReaction


#pragma endregion Combat
};