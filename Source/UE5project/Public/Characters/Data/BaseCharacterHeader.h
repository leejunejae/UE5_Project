// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseCharacterHeader.generated.h"


UENUM(BlueprintType)
enum class EBodyType : uint8
{
	Hand_R UMETA(DisplayName = "Hand_R"),
	Hand_L UMETA(DisplayName = "Hand_L"),
	Foot_R UMETA(DisplayName = "Foot_R"),
	Foot_L UMETA(DisplayName = "Foot_L"),
};

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Ground UMETA(DisplayName = "Ground"),
	Ladder UMETA(DisplayName = "Ladder"),
	Ride UMETA(DisplayName = "Ride"),
};

UENUM(BlueprintType)
enum class EGroundStance : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Walk UMETA(DisplayName = "Walk"),
	Jog UMETA(DisplayName = "Jog"),
	Sprint UMETA(DisplayName = "Sprint"),
};

UENUM(BlueprintType)
enum class ELadderStance : uint8
{
	Enter_From_Bottom UMETA(DisplayName = "Enter_From_Bottom"),
	Enter_From_Top UMETA(DisplayName = "Enter_From_Top"),
	Idle UMETA(DisplayName = "Idle"),
	Idle_OneStep UMETA(DisplayName = "Idle_OneStep"),
	ClimbUp UMETA(DisplayName = "ClimbUp"),
	ClimbUp_OneStep UMETA(DisplayName = "ClimbUp_OneStep"),
	ClimbDown UMETA(DisplayName = "ClimbDown"),
	ClimbDown_OneStep UMETA(DisplayName = "ClimbDown_OneStep"),
	Exit_From_Bottom UMETA(DisplayName = "Exit_From_Bottom"),
	Exit_From_Top UMETA(DisplayName = "Exit_From_Top"),
};

UENUM(BlueprintType)
enum class ERideStance : uint8
{
	Mount UMETA(DisplayName = "Mount"),
	DisMount UMETA(DisplayName = "DisMount"),
	Riding UMETA(DisplayName = "Riding"),
};

UCLASS()
class UE5PROJECT_API ABaseCharacterHeader : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseCharacterHeader();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
