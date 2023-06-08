// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "PBCharacter.generated.h"

class USkeletalMeshComponent;
class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCharacterMovementComponent;

UCLASS()
class UE5PROJECT_API APBCharacter : public ACharacter
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
		Normal, // ����¼�
		Combat // �����¼�
	};
	// ���� ���� ����
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

	UPROPERTY(VisibleAnywhere, Category = Camera)
		USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		UCameraComponent* Camera;

	/* ĳ���� �Է� ���� ���� */
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

	/* ĳ������ �⺻���� �������� �����ϴ� �Լ�*/
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
/* Public FUNCTION */
};