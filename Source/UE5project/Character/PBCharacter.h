// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "../PBDamagableInterface.h"
#include "../PBDamageSystem.h"
#include "PBCharacter.generated.h"

class USkeletalMeshComponent;
class UStaticMeshComponent;
class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCharacterMovementComponent;
class UPBDamageSystem;

UCLASS()
class UE5PROJECT_API APBCharacter : public ACharacter, public IPBDamagableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APBCharacter();

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
	enum class CharacterReadiness
	{
		Normal, // 평시태세
		Combat // 전투태세
	};
	// 연속 점프 제한
	//FTimerHandle JumpTimerHandle;
	//bool JumpHold;
	//int32 JumpTime;
/* PRIVATE VARIATION */

/* PRIVATE FUNCTION */
	//void JumpTimer();
	void Initialization();
/* PRIVATE FUNCTION */




/* PROTECTED VARIATION */
protected:
	CharacterReadiness CurrentReadiness;

	UPROPERTY(VisibleAnywhere, Category = Equipment)
		UStaticMeshComponent* Weapon;

	UPROPERTY(VisibleAnywhere, Category = Equipment)
		UStaticMeshComponent* SubEquip;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = Combat)
		UPBDamageSystem* DamageSystem;

	/* 캐릭터 입력 매핑 변수 */
	UPROPERTY(VisibleAnywhere, Category = Input)
		UInputMappingContext* DefaultContext;

	UPROPERTY(VisibleAnywhere, Category = Input)
		UInputAction* MoveAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
		UInputAction* LookAction;

	//UPROPERTY(VisibleAnywhere, Category = Input)
		//UInputAction* JumpAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
		UInputAction* AttackAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
		UInputAction* MoveSpeedToggleAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
		UInputAction* ReadinessToggleAction;


	FVector2D AimOffVal;

	bool IsRun;
	bool IsAttack;
/* PROTECTED VARIATION */

/* PROTECTED FUNCTION */
protected:

	/* 캐릭터의 기본적인 움직임을 수행하는 함수*/
	void Move(const FInputActionValue& value);
	void Look(const FInputActionValue& value);
	void ReadinessToggle();
	void CameraSetting();
	//void Jump();
	virtual void Attack();
	virtual void MoveSpeedToggle();

/* PROTECTED FUNCTION */



/* Public VARIATION */
public:

/* Public VARIATION */

/* Public FUNCTION */
public:
	bool ReturnReadiness();

	virtual bool TakeDamage_Implementation(FDamageInfo DamageInfo) override;
	virtual float Heal_Implementation(float amount) override;
	virtual float GetHealth_Implementation() override;
	virtual float GetMaxHealth_Implementation() override;

	UFUNCTION()
		virtual void Death();
	UFUNCTION()
		virtual void Block(bool CanParried);
	UFUNCTION()
		virtual void DamageResponse(HitResponse Response);
/* Public FUNCTION */
};