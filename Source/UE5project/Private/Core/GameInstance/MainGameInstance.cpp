// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameInstance/MainGameInstance.h"
#include "Core/Subsystems/WeaponDataSubsystem.h"
#include "Core/Subsystems/EnemyDataSubsystem.h"
//#include "UObject/SoftObjectPath.h"

void UMainGameInstance::Init()
{
	Super::Init();

    UWeaponDataSubsystem* WeaponSubsystem = GetSubsystem<UWeaponDataSubsystem>();
    if (WeaponSubsystem)
    {
        UE_LOG(LogTemp, Log, TEXT("WeaponSubSystem Loaded Successful"));
    }


    UEnemyDataSubsystem* EnemyDataSubsystem = GetSubsystem<UEnemyDataSubsystem>();
    if (EnemyDataSubsystem)
    {
        UE_LOG(LogTemp, Log, TEXT("EnemyDataSubSystem Loaded Successful"));
    }
}