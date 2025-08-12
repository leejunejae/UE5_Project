// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/EnemyBaseAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/KismetMathLibrary.h"

const FName AEnemyBaseAIController::PrevPos(TEXT("PrevPos"));
const FName AEnemyBaseAIController::NextPos(TEXT("NextPos"));
const FName AEnemyBaseAIController::TargetKey(TEXT("Target"));
const FName AEnemyBaseAIController::MeleeRadius(TEXT("MeleeRadius"));
const FName AEnemyBaseAIController::RangedRadius(TEXT("RangedRadius"));
const FName AEnemyBaseAIController::DefendRadius(TEXT("DefendRadius"));

AEnemyBaseAIController::AEnemyBaseAIController()
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

void AEnemyBaseAIController::BeginPlay()
{
	Super::BeginPlay();

	//AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ThisClass::OnTargetPerceptionUpdated_Delegate);
}

void AEnemyBaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	/*
	UBlackboardComponent* BlackboardComp = Blackboard.Get();
	if (!UseBlackboard(CachedBB, BlackboardComp))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed Using BlackBoard"));
		return;
	}

	Blackboard->SetValueAsVector(PrevPos, InPawn->GetActorLocation());
	if (!RunBehaviorTree(CachedBT))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed Running BehaviorTree"));
	}
	*/
}

void AEnemyBaseAIController::SetControllerData(UBehaviorTree* BehaviorTree, UBlackboardData* BlackboardData)
{
	UBlackboardComponent* BlackboardComp = Blackboard.Get();

	CachedBT = BehaviorTree;
	CachedBB = BlackboardData;

	if (!UseBlackboard(CachedBB, BlackboardComp))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed Using BlackBoard"));
		return;
	}

	if (!RunBehaviorTree(CachedBT))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed Running BehaviorTree"));
	}
}

void AEnemyBaseAIController::OnTargetPerceptionUpdated_Delegate(AActor* Actor, FAIStimulus Stimulus)
{
	Blackboard->SetValueAsObject(TargetKey, Actor);
	AttackTarget = Actor;
}

/*
void AEnemyBaseAIController::OnUnPossess()
{
	Super::OnUnPossess();
	GetWorld()->GetTimerManager().ClearTimer(RepeatTimerHandle);
}
*/

/*
void AEnemyBaseAIController::OnRepeatTimer()
{
	auto CurrentPawn = GetPawn();
	if (nullptr == CurrentPawn)
		return;

	//����� ��ġ�� ����޽��� ���´�
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (NavSystem == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("NOT FIND NAV MESH"));
		return;
	}

	FNavLocation NextLocation;
	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.0f, NextLocation))
	{
		//��ǥ ��ġ �α׸� ǥ���Ѵ�.
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, NextLocation.Location);
		UE_LOG(LogTemp, Warning, TEXT("Next Location: %s"), *NextLocation.Location.ToString());
	}
}
*/