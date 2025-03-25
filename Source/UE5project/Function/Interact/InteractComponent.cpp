// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include <Engine/Classes/Components/CapsuleComponent.h>
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "InteractInterface.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UInteractComponent::UInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

AActor* UInteractComponent::GetInteractActor()
{
	return InteractActor;
}

bool UInteractComponent::CheckInteractListValid()
{
	return InteractableList.IsEmpty() ? false : true;
}

void UInteractComponent::AddInteractObject(AActor* InteractObject)
{
	InteractableList.Add(InteractObject);
}

void UInteractComponent::RemoveInteractObject(AActor* InteractObject)
{
	InteractableList.Remove(InteractObject);
}

bool UInteractComponent::SetInteractActorByDegree(AActor* StartActor, float SearchDegrees)
{
	if (!CheckInteractListValid())
		return nullptr;

	AActor* TargetActor = nullptr;
	float CurrentDegrees = 60.0f;

	FVector CharLocation = StartActor->GetActorLocation();
	FVector ForwardVector = StartActor->GetActorForwardVector();

	for (auto& Act : InteractableList)
	{
		FVector ActorLocation = Act->GetActorLocation();

		FVector TargetDir = ActorLocation - CharLocation;
		TargetDir.Z = 0.0f;
		TargetDir.Normalize();

		float DotToTarget = FVector::DotProduct(ForwardVector, TargetDir);
		float RadianToTarget = FMath::Acos(DotToTarget);
		float AngleDegrees = FMath::RadiansToDegrees(RadianToTarget);

		if (AngleDegrees < CurrentDegrees)
		{
			CurrentDegrees = AngleDegrees;
			TargetActor = Act;
		}
	}

	InteractActor = TargetActor;

	return InteractActor != nullptr;
}

void UInteractComponent::MovetoInteractPos()
{
	UE_LOG(LogTemp, Error, TEXT("Interact4"));
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (Character == nullptr || InteractActor == nullptr)
		return;

	UE_LOG(LogTemp, Error, TEXT("Interact5"));
	USceneComponent* Target = IInteractInterface::Execute_GetEnterInteractLocation(InteractActor, Character);
	FVector DestLoc = Target->GetComponentLocation();
	FRotator DestRot = UKismetMathLibrary::FindLookAtRotation(Character->GetActorLocation(), DestLoc);
	Character->SetActorRotation(FRotator(0.0f, DestRot.Yaw, 0.0f));
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(Character->GetController(), DestLoc);

	InteractTimerDelegate.BindUObject(this, &UInteractComponent::InteractPosCheckTimer, Target);
	GetOwner()->GetWorldTimerManager().SetTimer(InteractTimerHandle, InteractTimerDelegate, 0.1f, true);
}

void UInteractComponent::InteractPosCheckTimer(USceneComponent* Target)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (Character == nullptr)
		return;

	FVector2D CharLoc = FVector2D(Character->GetActorLocation().X, Character->GetActorLocation().Y);
	FVector2D TargetLoc2D = FVector2D(Target->GetComponentLocation().X, Target->GetComponentLocation().Y);
	float Distance = FVector2D::Distance(CharLoc, TargetLoc2D);
	if (Distance < 50.0f)
	{
		Character->GetController()->StopMovement();
		FLatentActionInfo LatentInfo;
		LatentInfo.CallbackTarget = this;
		LatentInfo.Linkage = 0;
		LatentInfo.UUID = __LINE__;

		UKismetSystemLibrary::MoveComponentTo(
			Character->GetCapsuleComponent(),
			FVector(Target->GetComponentLocation().X, Target->GetComponentLocation().Y, Target->GetComponentLocation().Z + 92.0f),
			Target->GetComponentRotation(),
			false,
			false,
			0.1f,
			false,
			EMoveComponentAction::Type::Move,
			LatentInfo
		);

		//SetActorLocation(Target->GetComponentLocation());
		//SetActorRotation(Target->GetComponentRotation());
		OnArrivedInteractionPoint.ExecuteIfBound();

		GetOwner()->GetWorldTimerManager().ClearTimer(InteractTimerHandle);
	}
}
