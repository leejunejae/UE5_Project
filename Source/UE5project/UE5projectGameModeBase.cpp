// Copyright Epic Games, Inc. All Rights Reserved.


#include "UE5projectGameModeBase.h"
#include "PCWarrior.h"
#include "PBCController.h"

AUE5projectGameModeBase::AUE5projectGameModeBase()
{
	DefaultPawnClass = APCWarrior::StaticClass();
	PlayerControllerClass = APBCController::StaticClass();
}

void AUE5projectGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}