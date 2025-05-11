// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../BaseCharacterHeader.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include <Engine/Classes/Components/CapsuleComponent.h>
#include "EnhancedInputComponent.h"
#include "../Function/PlayerInterface.h"
#include "../Function/Combat/PBDamagableInterface.h"
#include "../Function/Combat/PBDamageSystem.h"
#include "../Function/Interact/ClimbComponent.h"
#include "../Function/Interact/ClimbInterface.h"
#include "../Enemy/Human/PBEHuman.h"
#include "NavigationSystem.h"
#include "NavigationInvokerComponent.h"
#include "../Function/Interact/InteractComponent.h"
#include "../Function/ViewDataInterface.h"
#include "Sound/SoundCue.h" 
#include "MotionWarpingComponent.h"
#include "CharacterBase.generated.h"

class USkeletalMeshComponent;
class UStaticMeshComponent;
class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCharacterMovementComponent;
class UPBDamageSystem;
class UClimbComponent;
class UInteractComponent;

DECLARE_DELEGATE(FOnSingleDelegate);

UENUM(BlueprintType)
enum class CharState : uint8
{
	None UMETA(DisplayName = "None"),
	Block UMETA(DisplayName = "Block"),
	Dodge UMETA(DisplayName = "Dodge"),
	Invincible UMETA(DisplayName = "Invincible"),
	Parry UMETA(DislplayName = "Parry"),
};

UCLASS()
class UE5PROJECT_API ACharacterBase : public ACharacter, public IPBDamagableInterface, public IPlayerInterface, public IClimbInterface, public IViewDataInterface
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
		TSubclassOf<class UDefaultWidget> DefaultWidgetClass;

	UPROPERTY(EditAnywhere)
		class UDefaultWidget* DefaultWidget;

	UPROPERTY(EditAnywhere)
		class UNavigationInvokerComponent* NavigationInvokerComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MotionWarping", meta = (AllowPrivateAccess = "true"))
		class UMotionWarpingComponent* MotionWarpingComponent;


	float YAxisScale;
	float DebugUpdateInterval = 0.1f; // 디버깅 갱신 간격
	float TimeSinceLastDebugUpdate = 0.0f;
	FVector PastLastInputVector;
/* PRIVATE VARIATION */

/* PRIVATE FUNCTION */
private:
	void Initialization();
/* PRIVATE FUNCTION */


/* PROTECTED VARIATION */
protected:
	MovementDirection CurrentDirection;

	UPROPERTY(VisibleAnywhere, Category = Equipment)
		UStaticMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category = Equipment)
		UStaticMeshComponent* SubEquipMesh;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = Combat)
		UPBDamageSystem* DamageComponent;

	UPROPERTY(VisibleAnywhere, Category = Climb)
		UClimbComponent* ClimbComponent;

	UPROPERTY(VisibleAnywhere, Category = Interact)
		UInteractComponent* InteractComponent;

	/* ĳ���� �Է� ���� ���� */
	UPROPERTY(EditAnywhere, Category = Input)
		UInputMappingContext* DefaultContext;

	UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* CheckMoveAction;

	UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* HeavyAttackAction;

	UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* DodgeAction;

	UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* BlockAction;

	UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* WalkAction;

	UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* SwitchStanceAction;

	UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* SpawnRideAction;

	FVector2D AimOffVal;

	bool IsLocomotion = true;
	bool IsMovementInput;
	bool IsClimb;
	bool IsClimbUp;

	//float InputX, InputY;
	FVector InputVector;
	FVector DodgeVector;
	FVector DodgeDirection;

	bool IsAction;
	bool IsRun;
	bool IsAttack;
	bool IsRoll;
	bool CanDodge;
	bool IsDodge;
	bool IsBlock;
	bool IsInteraction;
	bool CanInput=true;
	float AttackRange;
	float Vertical;
	float Horizontal;
	float InputDirection;
	float TargetSpeed=300.0f;

	FCharacterInfo CharInfo;

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
	virtual void Block();
	virtual void SwitchStance();
	void CameraSetting();
	
	void Jump();
	void Landed(const FHitResult& Hit) override;
	virtual void Attack();

	virtual void Interact();

/* PROTECTED FUNCTION */

/* Public VARIATION */
public:

/* Public VARIATION */

/* Public FUNCTION */
public:
	virtual bool IsPlayer_Implementation();
	virtual TOptional<FVector> GetCharBoneLocation(FName BoneName);

	virtual void TakeDamage_Implementation(FAttackInfo DamageInfo) override;
	virtual float Heal_Implementation(float amount) override;
	virtual float GetHealth_Implementation() override;
	virtual float GetMaxHealth_Implementation() override;

	UFUNCTION()
		virtual void Death();
	UFUNCTION()
		virtual void Block(bool CanParried);

	bool GetIsMovementInput();
	float GetRideSpeed();
	float GetRideDirection();
	FVector GetInputDirection();

	// Get Charcter Stat
	FString GetName();
	int32 GetMaxHP();
	int32 GetOffensePower();
	int32 GetDefenseCap();
/* Public FUNCTION */

#pragma region Animation
private:
	UPROPERTY(VisibleAnywhere, Category = Animation)
		class UCharacterBaseAnimInstance* CharacterBaseAnim;


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
#pragma endregion

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

private:
	float Direction;

#pragma endregion

#pragma region Ladder
public:
	UClimbComponent* GetClimbComponent();
	void SetCanMovementInput(bool CanMove);
	virtual void SetNextGripDown_Implementation(FName BoneName, int32 Count) override;

protected:
	void DecideLadderStance();

	UPROPERTY(EditAnywhere)
		float MinGripInterval = 15.0f;
	UPROPERTY(EditAnywhere)
		float MaxGripInterval = 50.0f;
	UPROPERTY(EditAnywhere)
		float MinFirstGripHeight = 40.0f;

private:
	FGripNode1D* Grip1D_Hand_R;
	FGripNode1D* Grip1D_Hand_L;
	FGripNode1D* Grip1D_Foot_R;
	FGripNode1D* Grip1D_Foot_L;
	float ClimbDistance;
	bool CanMovementInput;

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
public:
	virtual void RegisterInteractableActor_Implementation(AActor* Interactable);
	virtual void DeRegisterInteractableActor_Implementation(AActor* Interactable);
	virtual void EndInteraction_Implementation(AActor* Interactable);

protected:
	void HandleArrivedInteractionPoint();
#pragma endregion 
};