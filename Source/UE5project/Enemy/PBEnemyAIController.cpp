// Fill out your copyright notice in the Description page of Project Settings.


#include "PBEnemyAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

APBEnemyAIController::APBEnemyAIController()
{
	//RepeatInterval = 3.0f;
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