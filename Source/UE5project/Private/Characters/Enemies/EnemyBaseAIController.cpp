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
	//RepeatInterval = 3.0f;
}

void AEnemyBaseAIController::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemyBaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	//GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &AEnemyBaseAIController::OnRepeatTimer, RepeatInterval, true);
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