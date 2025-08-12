// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/Human/PBEHumanAI.h"
#include "Characters/Enemies/Human/PBEHuman.h"
#include "Kismet/KismetMathLibrary.h"
#include "Characters/Player/CharacterBase.h"

APBEHumanAI::APBEHumanAI()
{

}

void APBEHumanAI::BeginPlay()
{
	Super::BeginPlay();

	
}

void APBEHumanAI::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void APBEHumanAI::SetMovementMode(float MovementSpeed)
{
	auto ControllingPawn = Cast<APBEHuman>(GetPawn());

	ControllingPawn->SetMovementSpeed(MovementSpeed);
}

void APBEHumanAI::StopAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BehaviorTreeComponent && BehaviorTreeComponent->IsRunning())
		BehaviorTreeComponent->StopTree();
}

void APBEHumanAI::UpdateControlRotation(float DeltaTime, bool bUpdatePawn)
{
	Super::UpdateControlRotation(DeltaTime, false);

	//Smooth and change the pawn rotation
	
	if (bUpdatePawn)
	{
		//Get pawn
		APawn* const MyPawn = GetPawn();
		//Get Pawn current rotation
		const FRotator CurrentPawnRotation = MyPawn->GetActorRotation();

		//Calculate smoothed rotation
		SmoothTargetRotation = UKismetMathLibrary::RInterpTo_Constant(MyPawn->GetActorRotation(), ControlRotation, DeltaTime, SmoothFocusInterpSpeed);
		//Check if we need to change
		if (CurrentPawnRotation.Equals(SmoothTargetRotation, 1e-3f) == false)
		{
			//Change rotation using the Smooth Target Rotation
			MyPawn->FaceRotation(SmoothTargetRotation, DeltaTime);
		}
	}
}