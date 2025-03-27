// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "Components/BoxComponent.h"
#include "../PEnumHeader.h"
#include "../Function/Interact/InteractInterface.h"
#include "../Function/Interact/Ride/RideInterface.h"
#include "GameplayTagContainer.h"
#include "Ride.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UBoxComponent;

UENUM(BlueprintType)
enum class HorseDirection : uint8
{
	Back UMETA(DisplayName = "Back"),
	Stop UMETA(DisplayName = "Stop"),
	Front UMETA(DisplayName = "Front"),
};

UCLASS()
class UE5PROJECT_API ARide : public ACharacter, public IInteractInterface, public IRideInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARide();

private:
	void InputSetting();

	UFUNCTION()
		void TriggerBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void TriggerEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY(VisibleAnywhere, Category = Equipment)
		USkeletalMeshComponent* Saddle;
	UPROPERTY(VisibleAnywhere, Category = Equipment)
		USkeletalMeshComponent* Reins;

	UPROPERTY(EditAnywhere)
		class UWidgetComponent* InteractWidget;

	float VerticalTarget = 0.0f;
	UPROPERTY(VisibleAnywhere, Category = Velocity)
		float Vertical;
	float Horizontal;
	float WidgetAlpha = 0.0f;
	bool CanInteraction = false;
	bool IsBreak;
	bool MountRight;

	UPROPERTY(VisibleAnywhere, Category = Velocity)
	HorseDirection CurDirection;
	UPROPERTY(VisibleAnywhere, Category = Velocity)
	HorseMovement CurMovement;

	ACharacter* Rider;
	FRotator RiderRotator;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& value);
	void Look(const FInputActionValue& value);
	void Stop();
	void MoveSpeedToggle();
	void Interact();
	bool FindMountPos();

protected:
	UPROPERTY(VisibleAnywhere, Category = Camera)
		USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		UCameraComponent* Camera;

	/* �� �Է� */
	UPROPERTY(VisibleAnywhere, Category = Input)
		UInputMappingContext* DefaultContext;

	UPROPERTY(VisibleAnywhere, Category = Input)
		UInputAction* MoveAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
		UInputAction* LookAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
		UInputAction* MoveSpeedToggleAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
		UInputAction* InteractAction;

	/* �� �Է� */

	UPROPERTY(VisibleAnywhere, Category = Interact)
		UBoxComponent* RiderTrigger;

	UPROPERTY(VisibleAnywhere, Category = Interact)
		USceneComponent* RiderLocation1;

	UPROPERTY(VisibleAnywhere, Category = Interact)
		USceneComponent* RiderGetDownLoc;

	UPROPERTY(VisibleAnywhere, Category = Interact)
		USceneComponent* RiderMountLocLeft;

	UPROPERTY(VisibleAnywhere, Category = Interact)
		USceneComponent* RiderMountLocRight;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

	float CheckVar(RideVar CheckVar);
	bool CheckBool(RideVar CheckVar);

	virtual void RegisterInteractActor_Implementation(ACharacter* InteractActor);
	virtual void Interact_Implementation(ACharacter* InteractActor);
	virtual USceneComponent* GetEnterInteractLocation_Implementation(AActor* Target);
	virtual USceneComponent* GetLeftInteractLocation_Implementation();
	virtual FComponentTransform GetCameraData_Implementation();

	virtual float GetRideVertical_Implementation();
	virtual float GetRideHorizontal_Implementation();
	virtual bool GetMountDir_Implementation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
		FGameplayTagContainer RideTags;
};
