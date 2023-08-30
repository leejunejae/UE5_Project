// Fill out your copyright notice in the Description page of Project Settings.


#include "PBEnemyAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/KismetMathLibrary.h"

const FName APBEnemyAIController::PrevPos(TEXT("PrevPos"));
const FName APBEnemyAIController::NextPos(TEXT("NextPos"));
const FName APBEnemyAIController::TargetKey(TEXT("Target"));
const FName APBEnemyAIController::MeleeRadius(TEXT("MeleeRadius"));
const FName APBEnemyAIController::RangedRadius(TEXT("RangedRadius"));
const FName APBEnemyAIController::DefendRadius(TEXT("DefendRadius"));

APBEnemyAIController::APBEnemyAIController()
{
	//RepeatInterval = 3.0f;
}

void APBEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	
}

void APBEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	//GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &APBEnemyAIController::OnRepeatTimer, RepeatInterval, true);
}

/*
void APBEnemyAIController::OnUnPossess()
{
	Super::OnUnPossess();
	GetWorld()->GetTimerManager().ClearTimer(RepeatTimerHandle);
}
*/

/*
void APBEnemyAIController::OnRepeatTimer()
{
	auto CurrentPawn = GetPawn();
	if (nullptr == CurrentPawn)
		return;

	//월드상에 배치된 내비메쉬를 얻어온다
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (NavSystem == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("NOT FIND NAV MESH"));
		return;
	}

	FNavLocation NextLocation;
	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.0f, NextLocation))
	{
		//목표 위치 로그를 표기한다.
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, NextLocation.Location);
		UE_LOG(LogTemp, Warning, TEXT("Next Location: %s"), *NextLocation.Location.ToString());
	}
}
*/