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
	void RegisterClimbObject(AActor* RegistObject);
	void DeRegisterClimbObject();
	AActor* GetClimbObject();

#pragma endregion

#pragma region Grip And FootHold
protected:
	TArray<FGripNode1D> GripList1D;
	TArray<FGripNode2D> GripList2D;

public:
	void SetGrip1DRelation(float MinInterval, float MaxInterval);
	bool CheckGripListValid();

	/// <summary>
	/// Getter Function For Find Grip about various rule
	/// </summary>

	FGripNode1D* GetLowestGrip1D();
	FGripNode1D* GetHighestGrip1D();

	FGripNode1D* GetGripByHeightUpWard(float MinHeight = 0.0f, float Comparison = 0.0f);
	FGripNode1D* GetGripByHeightDownWard(float MinHeight = 0.0f, float Comparison = 0.0f);

	FGripNode1D* GetGripNeighborUpByRange(const FGripNode1D* CurrentGrip, float Range = 0);
	FGripNode1D* GetGripNeighborDownByRange(const FGripNode1D* CurrentGrip, float Range = 0);

	FGripNode1D* GetGripNeighborUp(const FGripNode1D* CurrentGrip, int32 Count = 1);
	FGripNode1D* GetGripNeighborDown(const FGripNode1D* CurrentGrip, int32 Count = 1);

	FGripNode1D* GetGripUpward(const FGripNode1D* CurrentGrip, float MinInterval = 0);
	FGripNode1D* GetGripDownward(const FGripNode1D* CurrentGrip, float MinInterval = 0);

	TOptional<int32> FindGripLevelDifference(const FGripNode1D* StartGrip, const FGripNode1D* DestGrip);
	TOptional<float> FindGripDistance(const FGripNode1D* StartGrip, const FGripNode1D* DestGrip);

	void SetLowestGrip1D(float MinHeight = 0.0f, float Comparision = 0.0f);

	void SetGripNeighborUp(FGripNode1D*& CurrentGrip, int32 Count = 1);
	void SetGripNeighborDown(FGripNode1D*& CurrentGrip, int32 Count = 1);

#pragma region Setting Value
private:
	float MinFirstGripHeight = 0.0f;
	float MinGripInterval = 0.0f;
	float MaxGripInterval = TNumericLimits<float>::Max();

/// <summary>
/// Setter Function For Setting Value
/// </summary>
public:
	void SetMinFirstGripHeight(float MinValue);
	void SetMinGripInterval(float MinInterval);
	void SetMaxGripInterval(float MaxInterval);

#pragma endregion

#pragma region Ladder Climbing
public:	
	TOptional<FTransform> GetEnterTopPosition();
	TOptional<FTransform> GetInitTopPosition();
	TOptional<FTransform> GetInitBottomPosition();
	float GetLadderTopTransitionDistance();

private:
	AActor* ClimbObject;
#pragma endregion
};
