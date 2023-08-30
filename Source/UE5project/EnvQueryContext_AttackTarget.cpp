// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvQueryContext_AttackTarget.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "Enemy/Human/PBEHumanAI.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

UEnvQueryContext_AttackTarget::UEnvQueryContext_AttackTarget()
{

}

void UEnvQueryContext_AttackTarget::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);
	//AActor* TestPawn = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass());
	//UEnvQueryItemType_Actor::SetContextHelper(ContextData, TestPawn);

	AActor* QueryOwner = Cast<AActor>(QueryInstance.Owner.Get());
	APawn* Player = UGameplayStatics::GetPlayerPawn(QueryOwner->GetWorld(), 0);
	APlayerController* ControllingPawn= UGameplayStatics::GetPlayerController(QueryOwner->GetWorld(), 0);
	if (ControllingPawn)
	{
		UEnvQueryItemType_Actor::SetContextHelper(ContextData, Cast<AActor>(Player));
	}
}