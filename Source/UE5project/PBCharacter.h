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

protected:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = Input)
	UInputMappingContext* DefaultContext;

	UPROPERTY(VisibleAnywhere, Category = Input)
	UInputAction* MoveAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	UInputAction* LookAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	UInputAction* JumpAction;

	/* 캐릭터의 기본적인 움직임을 수행하는 함수*/
	void Move(const FInputActionValue& value);
	void Look(const FInputActionValue& value);
	void CameraSetting();
	void Jump();

private:
	int32 MoveFlag;

	// 연속 점프 제한
	FTimerHandle JumpTimerHandle;
	bool JumpHold;
	int32 JumpTime;

	void JumpTimer();
	void Initialization();

public:
	int32 CheckMFlag()
	{
		return MoveFlag;
	}
};