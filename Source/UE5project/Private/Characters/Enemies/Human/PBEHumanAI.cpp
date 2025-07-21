// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/Human/PBEHumanAI.h"
#include "Characters/Enemies/Human/PBEHuman.h"
#include "Kismet/KismetMathLibrary.h"
#include "Characters/Player/CharacterBase.h"

APBEHumanAI::APBEHumanAI()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("PerceptionComponent");
	AISenseConfigSight = CreateDefaultSubobject<UAISenseConfig_Sight>("SenseSight");
	AISenseConfigSight->DetectionByAffiliation.bDetectEnemies = true;
	AISenseConfigSight->DetectionByAffiliation.bDetectFriendlies = true;
	AISenseConfigSight->DetectionByAffiliation.bDetectNeutrals = true;
	AISenseConfigSight->SightRadius = 5000.0f;
	AISenseConfigSight->LoseSightRadius = 10000.0f;
	AISenseConfigSight->PeripheralVisionAngleDegrees = 180.0f;

	AIPerceptionComponent->ConfigureSense(*AISenseConfigSight);
	AIPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());
}

void APBEHumanAI::BeginPlay()
{
	Super::BeginPlay();

	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ThisClass::OnTargetPerceptionUpdated_Delegate);
}

void APBEHumanAI::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	auto ControllingPawn = Cast<APBEHuman>(GetPawn());

	UBlackboardComponent* BlackboardComp = Blackboard.Get();
	if (!UseBlackboard(BBAsset, BlackboardComp))
		return;

	Blackboard->SetValueAsVector(PrevPos, InPawn->GetActorLocation());
	if (!RunBehaviorTree(BTAsset))
	{
		UE_LOG(LogTemp, Error, TEXT("AIController"));
	}
}

void APBEHumanAI::OnTargetPerceptionUpdated_Delegate(AActor* Actor, FAIStimulus Stimulus)
{
	UBlackboardComponent* BlackboardComp = Blackboard.Get();
	ACharacterBase* PlayerCharacter = Cast<ACharacterBase>(Actor);
	if (UseBlackboard(BBAsset, BlackboardComp) && PlayerCharacter && PlayerCharacter->GetController()->IsPlayerController())
	{
		Blackboard->SetValueAsObject(TargetKey, Actor);
		AttackTarget = Actor;
		if (!RunBehaviorTree(BTAsset))
		{
			UE_LOG(LogTemp, Error, TEXT("AIController"));
		}
	}
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