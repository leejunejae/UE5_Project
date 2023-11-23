// Fill out your copyright notice in the Description page of Project Settings.


#include "PBEHumanAI.h"
#include "PBEHuman.h"
#include "../../Character/PBCharacter.h"

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
	APBCharacter* PlayerCharacter = Cast<APBCharacter>(Actor);
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

void APBEHumanAI::SetMovementMode(MovementMode Mode)
{
	auto ControllingPawn = Cast<APBEHuman>(GetPawn());

	float movespeed;

	switch (Mode)
	{
	case MovementMode::Idle:
		movespeed = 0.0f;
		break;
	case MovementMode::Walking:
		movespeed = 150.0f;
		break;
	case MovementMode::Jogging:
		movespeed = 300.0f;
		break;
	case MovementMode::Sprinting:
		movespeed = 500.0f;
		break;
	default:
		movespeed = 0.0f;
		break;
	}

	ControllingPawn->SetMovementSpeed(movespeed);
}

void APBEHumanAI::StopAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BehaviorTreeComponent && BehaviorTreeComponent->IsRunning())
		BehaviorTreeComponent->StopTree();
}