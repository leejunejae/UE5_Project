// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameInstance/MainGameInstance.h"
#include "Core/Subsystems/WeaponDataSubsystem.h"
//#include "UObject/SoftObjectPath.h"

void UMainGameInstance::Init()
{
	Super::Init();

    UWeaponDataSubsystem* WeaponSubsystem = GetSubsystem<UWeaponDataSubsystem>();
    if (WeaponSubsystem)
    {
        UE_LOG(LogTemp, Log, TEXT("WeaponSubSystem Loaded Successful"));
        /*
        const FWeaponSetsInfo* WeaponSetInfo = WeaponSubsystem->GetWeaponInfo(FName("SwordShieldSet"));
        if (WeaponSetInfo)
        {
            UE_LOG(LogTemp, Log, TEXT("GameInstance Init: Loaded Weapon Set: %s"), *WeaponSetInfo->WeaponName.ToString());
        }
        */
    }
}