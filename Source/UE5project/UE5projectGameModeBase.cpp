// Copyright Epic Games, Inc. All Rights Reserved.


#include "UE5projectGameModeBase.h"
#include "Character/Warrior/FallenKnight.h"
#include "Character/ControllerBase.h"
#include "Function/DefaultWidget.h"

AUE5projectGameModeBase::AUE5projectGameModeBase()
{
	DefaultPawnClass = AFallenKnight::StaticClass();
	PlayerControllerClass = AControllerBase::StaticClass();
}

void AUE5projectGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}