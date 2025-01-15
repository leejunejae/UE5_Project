// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../BaseCharacterHeader.h"
#include "../../ClimbHeader.h"
#include "ClimbComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5PROJECT_API UClimbComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UClimbComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

#pragma region Climbable Object
public:
	TTuple<FVector, bool> SearchClimbTarget(FVector Start, FVector End, bool DebugTrace = false);

#pragma endregion

#pragma region Grip And FootHold
protected:
	TArray<FGripNode1D> GripList1D;
	TArray<FGripNode2D> GripList2D;

public:
	void RegisterClimbObject(AActor* RegistObject);
	void DeRegisterClimbObject();
	AActor* GetClimbObject();

	void SetGrip1DRelation(float MinInterval, float MaxInterval);
	TOptional<FVector> GetExitLocation();
	bool CheckGripListValid();

	FGripNode1D* GetLowestGrip1D();
	FGripNode1D* GetHighestGrip1D();
	FGripNode1D* FindGripByHeight(float MinHeight = 0.0f, float Comparison = 0.0f);
	FGripNode1D* FindGripNeighborUp(const FGripNode1D* CurrentGrip, int32 Count = 0);
	FGripNode1D* FindGripNeighborDown(const FGripNode1D* CurrentGrip, int32 Count = 0);
	FGripNode1D* FindGripUpward(const FGripNode1D* CurrentGrip, float MinInterval = 0);
	FGripNode1D* FindGripDownward(const FGripNode1D* CurrentGrip, float MinInterval = 0);
	void SetGripNeighborUp(FGripNode1D* CurrentGrip, int32 Count = 0);
	void SetGripNeighborDown(FGripNode1D* CurrentGrip, int32 Count = 0);
	void SetGripUpward(FGripNode1D* CurrentGrip, float MinInterval = 0);
	void SetGripDownward(FGripNode1D* CurrentGrip, float MinInterval = 0);

	bool FindGripLocation(FVector* Target, FVector Start, float TraceDistance, float PassDistance, FName GripTag, bool DebugTrace = false);

#pragma endregion

#pragma region Ladder Climbing
public:	
	void SetELadderStance(ELadderStance StanceValue);
	ELadderStance GetELadderStance();

	USceneComponent* GetExitTarget();
	TOptional<FTransform> GetEnterTopPosition();
	TOptional<FTransform> GetInitTopPosition();
	TOptional<FTransform> GetInitBottomPosition();

private:
	AActor* ClimbObject;

	ELadderStance CurLadderStance;

#pragma endregion
};
