// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Subsystems/WorldSystem/WorldSpawnSubsystem.h"
#include "Utils/SpawnMarker.h"
#include "EngineUtils.h"

#include "Utils/CoreLog.h"

#include "Characters/Enemies/EnemyBase.h"
#include "Core/Subsystems/GameInstanceSystem/EnemyDataSubsystem.h"

void UWorldSpawnSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

}

void UWorldSpawnSubsystem::Deinitialize()
{
	MarkersBySpawnID.Empty();
	Super::Deinitialize();
}

void UWorldSpawnSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	EnemyDataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UEnemyDataSubsystem>();
	if (!EnemyDataSubsystem)
	{
		UE_LOG(Log_NPCSpawn, Error, TEXT("[WorldSpawnSubsystem] EnemyDataSubsystem not found."));
		return;
	}

	FTSTicker::GetCoreTicker().AddTicker(
		FTickerDelegate::CreateWeakLambda(this, [this](float)
			{
				for (auto& Pair : MarkersBySpawnID)
				{
					if (ASpawnMarker* M = Pair.Value.Get())
					{
						SpawnFromMarker(M);
						//UE_LOG(Log_NPCSpawn, Error, TEXT("[WorldSpawnSubsystem] EnemySpawn"));
					}
				}
				return false; // 한 번만
			}), 0.0f);
}

void UWorldSpawnSubsystem::RegisterMarker(ASpawnMarker* Marker)
{
	if (!Marker || !Marker->GetSpawnID().IsValid()) return;

	MarkersBySpawnID.Add(Marker->GetSpawnID(), Marker);
}

void UWorldSpawnSubsystem::UnregisterMarker(ASpawnMarker* Marker)
{
	if (!Marker) return;
	MarkersBySpawnID.Remove(Marker->GetSpawnID());
}

void UWorldSpawnSubsystem::SpawnFromMarker(ASpawnMarker* Marker)
{
	if (!Marker) return;

	const FEnemyDataSet* Data = EnemyDataSubsystem->GetEnemyData(Marker->GetEnemyID());

	if (!Data || !Data->Info || !Data->Stats)
	{
		UE_LOG(Log_NPCSpawn, Warning, TEXT("[WorldSpawnSubsystem] Missing Enemy data for %s"), *Marker->GetName());
		return;
	}

	UWorld* World = GetWorld();
	if (!World) return;

	AEnemyBase* Enemy = World->SpawnActorDeferred<AEnemyBase>(AEnemyBase::StaticClass(), Marker->GetActorTransform());

	if (!Enemy)
	{
		UE_LOG(Log_NPCSpawn, Warning, TEXT("[WorldSpawnSubsystem] Enemies did not spawn properly  %s"), *Marker->GetName());
		return;
	}

	Enemy->SetEnemyID(Marker->GetEnemyID());
	Enemy->SetDefaultRouteGuid(Marker->GetRouteID());
	Enemy->FinishSpawning(Marker->GetActorTransform());
	Enemy->ApplyEnemyInfo(Data->Info);
	Enemy->ApplyEnemyStats(Data->Stats);
}
