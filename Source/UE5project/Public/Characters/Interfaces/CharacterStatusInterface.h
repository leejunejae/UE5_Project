// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Characters/Data/StatusData.h"
#include "CharacterStatusInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCharacterStatusInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UE5PROJECT_API ICharacterStatusInterface
{
	GENERATED_BODY()
		
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		ECharacterGroundState GetGroundState() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void SetGroundState(ECharacterGroundState NewState);
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
};
