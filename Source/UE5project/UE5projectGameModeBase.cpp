// Copyright Epic Games, Inc. All Rights Reserved.


#include "UE5projectGameModeBase.h"
#include "Character/Warrior/Warrior.h"
#include "Character/ControllerBase.h"
#include "Function/PBDefaultWidget.h"

AUE5projectGameModeBase::AUE5projectGameModeBase()
{
	DefaultPawnClass = AWarrior::StaticClass();
	PlayerControllerClass = AControllerBase::StaticClass();
}

void AUE5projectGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}