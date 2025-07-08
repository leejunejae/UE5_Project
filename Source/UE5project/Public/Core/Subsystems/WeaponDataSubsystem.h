// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Items/Weapons/Data/WeaponData.h"
#include "Engine/DataTable.h"
#include "WeaponDataSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API UWeaponDataSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere)
		TObjectPtr<UDataTable> WeaponList = nullptr;
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;


	UPROPERTY(EditAnywhere)
		TObjectPtr<UWeaponDataAsset> WeaponDataAsset;

	const FWeaponSetsInfo* GetWeaponInfo(const FName& WeaponName) const;
};
