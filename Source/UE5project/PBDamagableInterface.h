// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PEnumHeader.h"
#include "PBDamagableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPBDamagableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UE5PROJECT_API IPBDamagableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		float GetHealth();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		float GetMaxHealth();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		float Heal(float amount);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool TakeDamage(FDamageInfo DamageInfo);
};
