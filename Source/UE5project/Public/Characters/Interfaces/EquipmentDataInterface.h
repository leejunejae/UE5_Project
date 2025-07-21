// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "Items/Weapons/Data/WeaponData.h"
#include "EquipmentDataInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEquipmentDataInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UE5PROJECT_API IEquipmentDataInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		FWeaponSetsInfo GetWeaponSetsData() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		FWeaponPartInfo GetMainWeaponData() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		FWeaponPartInfo GetSubEquipData() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		UStaticMeshComponent* GetMainWeaponMeshComponent() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		UStaticMeshComponent* GetSubEquipMeshComponent() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void EquipWeapon(FName WeaponKey);
};
