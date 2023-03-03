// Copyright Epic Games, Inc. All Rights Reserved.


#include "UE5projectGameModeBase.h"
#include "PBCharacter.h"
#include "PBCController.h"

AUE5projectGameModeBase::AUE5projectGameModeBase()
{
	DefaultPawnClass = APBCharacter::StaticClass();
	PlayerControllerClass = APBCController::StaticClass();
}

void AUE5projectGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}