// Fill out your copyright notice in the Description page of Project Settings.


#include "ClimbComponent.h"
#include "ClimbObjectInterface.h"
#include "LadderInterface.h"

// Sets default values for this component's properties
UClimbComponent::UClimbComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UClimbComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

TTuple<FVector, bool> UClimbComponent::SearchClimbTarget(FVector Start, FVector End, bool DebugTrace)
{
	TArray<FHitResult> HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.bTraceComplex = true;
	enum ECollisionChannel TraceChannel = ECC_Visibility;

	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResult,
		Start,
		End,
		FQuat::Identity,
		TraceChannel,
		FCollisionShape::MakeSphere(40.0f),
		CollisionParams
	);

	FVector TraceVec = End - Start;
	FVector Center = Start + TraceVec * 0.5f;
	float HalfHeight = FVector::Dist(Start, End) * 0.5f;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	float DebugLifeTime = 5.0f;

	for (FHitResult Result : HitResult)
	{
		if (Result.GetActor()->ActorHasTag(FName("Climbable")))
		{
			if (DebugTrace)
			{
				DrawDebugCapsule(
					GetWorld(),
					Center,
					HalfHeight,
					40.0f,
					CapsuleRot,
					FColor::Green,
					false,
					DebugLifeTime
				);
			}

			return MakeTuple(Result.ImpactPoint, true);
		}
	}

	return MakeTuple(FVector::ZeroVector, false);
}

void UClimbComponent::RegisterClimbObject(AActor* RegistObject)
{
	ClimbObject = RegistObject;
	//UE_LOG(LogTemp, Warning, TEXT("Register Climb Object"));
	if (ClimbObject->GetClass()->ImplementsInterface(UClimbObjectInterface::StaticClass()))
	{
		if (ClimbObject->ActorHasTag(FName("Ladder")))
		{
			GripList1D = IClimbObjectInterface::Execute_GetGripList1D(ClimbObject);
			if (!GripList1D.IsEmpty())
			{
				SetGrip1DRelation(MinGripInterval, MaxGripInterval);
				SetLowestGrip1D(MinFirstGripHeight, GetInitBottomPosition().GetValue().GetLocation().Z);
			}
		}
		/*
		if (GripList1D.IsEmpty())
		{
			UE_LOG(LogTemp, Warning, TEXT("GripList is Empty"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("GripList isnt Empty"));
		}
		*/
	}
}

void UClimbComponent::DeRegisterClimbObject()
{
	ClimbObject = nullptr;
	GripList1D.Empty();
}

AActor* UClimbComponent::GetClimbObject()
{
	return ClimbObject == nullptr ? nullptr : ClimbObject;
}

void UClimbComponent::SetMinFirstGripHeight(float MinValue)
{
	MinFirstGripHeight = MinValue;
}

void UClimbComponent::SetMinGripInterval(float MinInterval)
{
	MinGripInterval = MinInterval;
}

void UClimbComponent::SetMaxGripInterval(float MaxInterval)
{
	MaxGripInterval = MaxInterval;
}

USceneComponent* UClimbComponent::GetExitTarget()
{
	/*
	if (ClimbObject->GetClass()->ImplementsInterface(ULadderInterface::StaticClass()))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Object Has Climb Interface"));
		return ILadderInterface::Execute_GetExitTopLocation(ClimbObject);
	}
	*/
	return nullptr;
}

TOptional<FTransform> UClimbComponent::GetEnterTopPosition()
{
	if (ClimbObject->GetClass()->ImplementsInterface(ULadderInterface::StaticClass()))
	{
		USceneComponent* InitTopTarget = ILadderInterface::Execute_GetEnterTopTarget(ClimbObject);
		FTransform InitTopPosition = InitTopTarget->GetComponentTransform();
		return InitTopPosition;
	}

	return TOptional<FTransform>();
}

TOptional<FTransform> UClimbComponent::GetInitTopPosition()
{
	if (ClimbObject->GetClass()->ImplementsInterface(ULadderInterface::StaticClass()))
	{
		USceneComponent* InitTopTarget = ILadderInterface::Execute_GetInitTopTarget(ClimbObject);
		FTransform InitTopPosition = InitTopTarget->GetComponentTransform();
		return InitTopPosition;
	}

	return TOptional<FTransform>();
}

TOptional<FTransform> UClimbComponent::GetInitBottomPosition()
{
	if (ClimbObject->GetClass()->ImplementsInterface(ULadderInterface::StaticClass()))
	{
		USceneComponent* InitBottomTarget = ILadderInterface::Execute_GetInitBottomTarget(ClimbObject);
		FTransform InitBottomPosition = InitBottomTarget->GetComponentTransform();
		return InitBottomPosition;
	}

	return TOptional<FTransform>();
}

float UClimbComponent::GetLadderTopTransitionDistance()
{
	if (!GetInitTopPosition().IsSet() || !GetEnterTopPosition().IsSet())
		return 0.0f;

	const FVector2D InitLocation = FVector2D(GetInitTopPosition().GetValue().GetLocation().X, GetInitTopPosition().GetValue().GetLocation().Y);
	const FVector2D EnterLocation = FVector2D(GetEnterTopPosition().GetValue().GetLocation().X, GetEnterTopPosition().GetValue().GetLocation().Y);
	return FVector2D::Distance(InitLocation, EnterLocation);
}

TOptional<FVector> UClimbComponent::GetExitLocation()
{
	FVector ExitLocation = GetInitTopPosition().GetValue().GetLocation();
	//UE_LOG(LogTemp, Warning, TEXT("ExitTarget Location : X = %f, Y = %f, Z = %f"), ExitTarget->GetComponentLocation().X, ExitTarget->GetComponentLocation().Y, ExitTarget->GetComponentLocation().Z);

	float TraceDistance = 500.0f;
	FVector StartLoc = ExitLocation;
	FVector EndLoc = StartLoc - FVector(0.0f, 0.0f, TraceDistance);

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		StartLoc,
		EndLoc,
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeSphere(20.0f),
		CollisionParams
	);

	FVector TraceVec = EndLoc - StartLoc;
	FVector Center = StartLoc + TraceVec * 0.5f;
	float HalfHeight = FVector::Dist(StartLoc, EndLoc) * 0.5f;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bHit ? FColor::Green : FColor::Red;
	float DebugLifeTime = 5.0f;

	DrawDebugCapsule(
		GetWorld(),
		Center,
		HalfHeight,
		20.0f,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime
	);

	if (bHit)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Trace Hit"));
		return HitResult.ImpactPoint;
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Trace Doesnt Hit"));
		return TOptional<FVector>();
	}
}

void UClimbComponent::SetGrip1DRelation(float MinInterval, float MaxInterval)
{
	if (!CheckGripListValid())
		return;

	for (int32 i = 0; i < GripList1D.Num(); i++) 
	{
		//UE_LOG(LogTemp, Warning, TEXT("Current index = % f"), i);
		int32 lowerindex = i - 1;
		int32 upperindex = i + 1;

		while (GripList1D.IsValidIndex(lowerindex))
		{
			float DistanceToLowerGrip = FVector::Dist(GripList1D[i].Position, GripList1D[lowerindex].Position);
			if (DistanceToLowerGrip >= MinInterval && DistanceToLowerGrip <= MaxInterval)
			{
				GripList1D[i].NeighborDown = { &GripList1D[lowerindex], DistanceToLowerGrip };
				//UE_LOG(LogTemp, Warning, TEXT("Neighbors Position Z of GripList[%d] LowerGrip = %f"), i, GripList1D[lowerindex].Position.Z);
				break;
			}
			else if (DistanceToLowerGrip < MinInterval)
			{
				lowerindex--;
				continue;
			}
			else
			{
				break;
			}
		}

		while (GripList1D.IsValidIndex(upperindex))
		{
			float DistanceToUpperGrip = FVector::Dist(GripList1D[i].Position, GripList1D[upperindex].Position);
			if (DistanceToUpperGrip >= MinInterval && DistanceToUpperGrip <= MaxInterval)
			{
				GripList1D[i].NeighborUp = { &GripList1D[upperindex], DistanceToUpperGrip };
				//UE_LOG(LogTemp, Warning, TEXT("Neighbors Position Z of GripList[%d] UpperGrip = %f"), i, GripList1D[upperindex].Position.Z);
				//upperindex++;
				break;
			}
			else if (DistanceToUpperGrip < MinInterval)
			{
				upperindex++;
				continue;
			}
			else
			{
				break;
			}
		}	
	}
}

bool UClimbComponent::CheckGripListValid()
{
	return GripList1D.IsEmpty() ? false : true;
}

FGripNode1D* UClimbComponent::GetLowestGrip1D()
{
	if (!CheckGripListValid())
	{
		return nullptr;
	}

	for (FGripNode1D& GripNode : GripList1D)
	{
		if(GripNode.Tag.Contains(FName("LowestGrip")))
			return &GripNode;
	}

	return &GripList1D[0];
}

FGripNode1D* UClimbComponent::GetHighestGrip1D()
{
	if (!CheckGripListValid())
	{
		return nullptr;
	}

	return &GripList1D.Last();
}

FGripNode1D* UClimbComponent::GetGripByHeightUpWard(float MinHeight, float Comparison)
{
	for (FGripNode1D& GripNode : GripList1D)
	{
		if (GripNode.Position.Z - Comparison > MinHeight)
		{
			return &GripNode;
		}
	}

	return nullptr;
}

FGripNode1D* UClimbComponent::GetGripByHeightDownWard(float MinHeight, float Comparison)
{
	for (FGripNode1D& GripNode : GripList1D)
	{
		if (Comparison - GripNode.Position.Z > MinHeight)
		{
			return &GripNode;
		}
	}

	return nullptr;
}

FGripNode1D* UClimbComponent::GetGripNeighborUpByRange(const FGripNode1D* CurrentGrip, float Range)
{
	//check(CurrentGrip != nullptr);

	if (CurrentGrip == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentGrip is Nullptr By Function GetGripNeighborDownByRange"));
		return nullptr;
	}
	UE_LOG(LogTemp, Warning, TEXT("Current Index = %d"), CurrentGrip->GripIndex);
	UE_LOG(LogTemp, Warning, TEXT("Neighbor Distance = %f"), CurrentGrip->NeighborUp.Distance);
	UE_LOG(LogTemp, Warning, TEXT("Range = %f"), Range);
	UE_LOG(LogTemp, Warning, TEXT("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ"));
	if (CurrentGrip->NeighborUp.Distance < Range)
	{
		return GetGripNeighborUpByRange(CurrentGrip->NeighborUp.Neighbor, Range - CurrentGrip->NeighborUp.Distance);
	}

	//UE_LOG(LogTemp, Warning, TEXT("GripIndex : %d"), CurrentGrip->NeighborUp.Neighbor->GripIndex);
	return CurrentGrip->NeighborUp.Neighbor;
}

FGripNode1D* UClimbComponent::GetGripNeighborDownByRange(const FGripNode1D* CurrentGrip, float Range)
{
	if (CurrentGrip == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentGrip is Nullptr By Function GetGripNeighborDownByRange"));
		return nullptr;
	}
	//check(CurrentGrip != nullptr);
	//UE_LOG(LogTemp, Warning, TEXT("CurrentGrip Position: %d"), CurrentGrip->NeighborUp.Neighbor->GripIndex);
	UE_LOG(LogTemp, Warning, TEXT("Current Index = %d"), CurrentGrip->GripIndex);
	UE_LOG(LogTemp, Warning, TEXT("Neighbor Distance = %f"), CurrentGrip->NeighborDown.Distance);
	UE_LOG(LogTemp, Warning, TEXT("Range = %f"), Range);
	UE_LOG(LogTemp, Warning, TEXT("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ"));
	if (CurrentGrip->NeighborDown.Distance < Range)
	{
		return GetGripNeighborDownByRange(CurrentGrip->NeighborDown.Neighbor, Range - CurrentGrip->NeighborDown.Distance);
	}

	//UE_LOG(LogTemp, Warning, TEXT("GripIndex : %d"), CurrentGrip->NeighborUp.Neighbor->GripIndex);
	return CurrentGrip->NeighborDown.Neighbor;
}

FGripNode1D* UClimbComponent::GetGripNeighborUp(const FGripNode1D* CurrentGrip, int32 Count)
{	
	if (CurrentGrip == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentGrip is Nullptr By Function GetGripNeighborUp"));
		return nullptr;
	}
	//check(CurrentGrip->NeighborUp.Neighbor != nullptr)

	//UE_LOG(LogTemp, Warning, TEXT("Count : %d"), Count);
	if (Count > 1)
	{
		return GetGripNeighborUp(CurrentGrip->NeighborUp.Neighbor, Count - 1);
	}

	//UE_LOG(LogTemp, Warning, TEXT("GripIndex : %d"), CurrentGrip->NeighborUp.Neighbor->GripIndex);
	return CurrentGrip->NeighborUp.Neighbor;
}

FGripNode1D* UClimbComponent::GetGripNeighborDown(const FGripNode1D* CurrentGrip, int32 Count)
{
	if (CurrentGrip == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentGrip is Nullptr By Function GetGripNeighborDown"));
		return nullptr;
	}

	//UE_LOG(LogTemp, Warning, TEXT("Count : %d"), Count);
	if (Count > 1)
	{
		return GetGripNeighborDown(CurrentGrip->NeighborDown.Neighbor, Count - 1);
	}

	//UE_LOG(LogTemp, Warning, TEXT("GripIndex : %d"), CurrentGrip->GripIndex);
	return CurrentGrip->NeighborDown.Neighbor;
}

FGripNode1D* UClimbComponent::GetGripUpward(const FGripNode1D* CurrentGrip, float MinInterval)
{
	int32 SearchIndex = CurrentGrip->GripIndex + 1;
	
	while (GripList1D.IsValidIndex(SearchIndex))
	{
		float GripPositionDiff = FMath::Abs(GripList1D[SearchIndex].Position.Z - CurrentGrip->Position.Z);

		if (GripPositionDiff > MinInterval)
		{
			//UE_LOG(LogTemp, Warning, TEXT("LeftHand : %f"), GripPositionDiff);
			return &GripList1D[SearchIndex];
		}

		SearchIndex++;
	}

	return nullptr;
}

FGripNode1D* UClimbComponent::GetGripDownward(const FGripNode1D* CurrentGrip, float MinInterval)
{
	int32 SearchIndex = CurrentGrip->GripIndex - 1;

	while (GripList1D.IsValidIndex(SearchIndex))
	{
		float GripPositionDiff = FMath::Abs(CurrentGrip->Position.Z - GripList1D[SearchIndex].Position.Z);

		if (GripPositionDiff > MinInterval)
		{
			return &GripList1D[SearchIndex];
		}

		SearchIndex--;
	}

	return nullptr;
}

TOptional<int32> UClimbComponent::FindGripLevelDifference(const FGripNode1D* StartGrip, const FGripNode1D* DestGrip)
{
	if (StartGrip == nullptr || DestGrip == nullptr)
	{
		if (StartGrip == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("StartGrip Null"));
		}

		if (DestGrip == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("DestGrip Null"));
		}
		return TOptional<int32>();
	}
	

	return FMath::Abs(DestGrip->GripIndex - StartGrip->GripIndex);
}

TOptional<float> UClimbComponent::FindGripDistance(const FGripNode1D* StartGrip, const FGripNode1D* DestGrip)
{
	if (StartGrip == nullptr || DestGrip == nullptr)
		return TOptional<float>();

	return DestGrip->Position.Z - StartGrip->Position.Z;
}

void UClimbComponent::SetLowestGrip1D(float MinHeight, float Comparison)
{
	for (FGripNode1D& GripNode : GripList1D)
	{
		if (GripNode.Position.Z - Comparison > MinHeight)
		{
			GripNode.Tag.Add(FName("LowestGrip"));
			return;
		}
	}
}

void UClimbComponent::SetGripNeighborUpByRange(FGripNode1D* CurrentGrip, float Range)
{

}

void UClimbComponent::SetGripNeighborDownByRange(FGripNode1D* CurrentGrip, float Range)
{
}

void UClimbComponent::SetGripNeighborUp(FGripNode1D*& CurrentGrip, int32 Count)
{
	if (!CurrentGrip)
	{
		return;
	}

	FGripNode1D* PrevGrip = CurrentGrip;
	CurrentGrip = GetGripNeighborUp(CurrentGrip, Count);

	if (CurrentGrip)
	{
		CurrentGrip->PrevGrip = PrevGrip;
	}
}

void UClimbComponent::SetGripNeighborDown(FGripNode1D*& CurrentGrip, int32 Count)
{
	if (!CurrentGrip)
	{
		return;
	}

	FGripNode1D* PrevGrip = CurrentGrip;
	CurrentGrip = GetGripNeighborDown(CurrentGrip, Count);

	if (CurrentGrip)
	{
		CurrentGrip->PrevGrip = PrevGrip;
	}
}

bool UClimbComponent::SetGripLocation(FVector* Target, FVector Start, float TraceDistance, float PassDistance, FName GripTag, bool DebugTrace)
{
	FVector End = Start + FVector(0.0f, 0.0f, TraceDistance);

	TArray<FHitResult> HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.bTraceComplex = true;
	enum ECollisionChannel TraceChannel = ECC_GameTraceChannel9;

	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResult,
		Start,
		End,
		FQuat::Identity,
		TraceChannel,
		FCollisionShape::MakeSphere(15.0f),
		CollisionParams
	);

	FVector TraceVec = End - Start;
	FVector Center = Start + TraceVec * 0.5f;
	float HalfHeight = FVector::Dist(Start, End) * 0.5f;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	float DebugLifeTime = 5.0f;

	UE_LOG(LogTemp, Warning, TEXT("Number Of Hitting Collision : %d"), HitResult.Num());

	for (FHitResult Result : HitResult)
	{
		if (Result.Distance > PassDistance)
		{
			if (DebugTrace)
			{
				DrawDebugCapsule(
					GetWorld(),
					Center,
					HalfHeight,
					15.0f,
					CapsuleRot,
					FColor::Green,
					false,
					DebugLifeTime
				);
			}

			*Target = Result.GetComponent()->GetComponentLocation();

			return true;
		}
	}

	if (DebugTrace)
	{
		DrawDebugCapsule(
			GetWorld(),
			Center,
			HalfHeight,
			15.0f,
			CapsuleRot,
			FColor::Red,
			false,
			DebugLifeTime
		);
	}

	return false;
}

void UClimbComponent::SetELadderStance(ELadderStance StanceValue)
{
	CurLadderStance = StanceValue;
}

ELadderStance UClimbComponent::GetELadderStance()
{
	return CurLadderStance;
}
