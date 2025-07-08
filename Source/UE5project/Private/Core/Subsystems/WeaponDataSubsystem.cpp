// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SubSystems/WeaponDataSubsystem.h"
#include "UObject/ConstructorHelpers.h"

void UWeaponDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

	TSoftObjectPtr<UDataTable> SetsTableRef = TSoftObjectPtr<UDataTable>(FSoftObjectPath(TEXT("/Game/02_Item/Equipment/Weapon/Data/WeaponData_DT.WeaponData_DT")));
	if (SetsTableRef.IsValid() == false)
	{
		SetsTableRef.LoadSynchronous();
	}
	WeaponList = SetsTableRef.Get();
}

const FWeaponSetsInfo* UWeaponDataSubsystem::GetWeaponInfo(const FName& WeaponName) const
{
	if (WeaponList)
	{
		return WeaponList->FindRow<FWeaponSetsInfo>(WeaponName, TEXT(""));
	}

	return nullptr;
}
