// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Function/Interact/LadderInterface.h"
#include "ClimbableObjectBase.h"
#include "LadderBase.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API ALadderBase : public AClimbableObjectBase, public ILadderInterface
{
	GENERATED_BODY()
	
public:
	ALadderBase();

	virtual USceneComponent* GetExitTopLocation_Implementation();
	virtual USceneComponent* GetInitTopPosition_Implementation();

#pragma region Ladder Basic Composition
////////////////////////////////////
// Methods For Ladder Basic Composition
////////////////////////////////////
protected:
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

////////////////////////////////////
// Variables For Ladder Basic Composition
////////////////////////////////////
protected:
	UPROPERTY(VisibleAnywhere, Category = Interact)
		USceneComponent* ExitTopPosition;

	UPROPERTY(VisibleAnywhere, Category = Interact)
		USceneComponent* EnterTopPosition;

	UPROPERTY(EditAnywhere, Category = LadderSetting) // Layer for modular ladder
		int32 LadderLevel;

	UPROPERTY(EditAnywhere, Category = LadderSetting)
		FVector LadderScale;

	UPROPERTY(EditAnywhere, Category = LadderSetting)
		float AdditionalHeight;

	UPROPERTY(EditAnywhere, Category = Mesh)
		TArray<UStaticMeshComponent*> ClimbMeshes;

	UPROPERTY(EditAnywhere, Category = Mesh)
		TArray<UBoxComponent*> GripCollisions;
#pragma endregion
};
