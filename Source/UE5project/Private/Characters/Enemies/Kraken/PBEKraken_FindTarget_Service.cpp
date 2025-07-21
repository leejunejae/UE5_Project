// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Enemies/Kraken/PBEKraken_FindTarget_Service.h"
#include "Characters/Player/CharacterBase.h"
#include "Characters/Enemies/Kraken/PBEKraken.h"
#include "Characters/Enemies/Kraken/PBEKrakenAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UPBEKraken_FindTarget_Service::UPBEKraken_FindTarget_Service()
{
	NodeName = TEXT("FindTarget");
	Interval = 1.0f;
}

void UPBEKraken_FindTarget_Service::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto CurrentPawn = Cast<APBEKraken>(OwnerComp.GetAIOwner()->GetPawn());
	if (CurrentPawn == nullptr)
		return;

	UWorld* World = CurrentPawn->GetWorld();
	FVector Center = CurrentPawn->GetActorLocation();
	float SearchRadius = 1500.0f;

	if (World == nullptr)
		return;
	/*
	if (CurrentPawn->ZombieDead())
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName(TEXT("Dead")), true);
		return;
	}
	*/

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams(NAME_None, false, CurrentPawn);

	APBEKrakenAI* KrakenAI = Cast<APBEKrakenAI>(OwnerComp.GetAIOwner());


	// DrawDebugSphere(World, Center, SearchRadius, 16, FColor::Red, false, 0.2f);

	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(SearchRadius),
		QueryParams);

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			ACharacterBase* PlayerCharacter = Cast<ACharacterBase>(OverlapResult.GetActor());
			if (PlayerCharacter && PlayerCharacter->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), PlayerCharacter);

				DrawDebugSphere(World, Center, SearchRadius, 16, FColor::Green, false, 0.2f);
				return;
			}
		}
	}

	DrawDebugSphere(World, Center, SearchRadius, 16, FColor::Red, false, 0.2f);
}