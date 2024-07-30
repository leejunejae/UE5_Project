// Fill out your copyright notice in the Description page of Project Settings.


#include "PBDefaultWidget.h"
#include "Components/WidgetSwitcher.h"

UPBDefaultWidget::UPBDefaultWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UPBDefaultWidget::SetUp()
{
	this->AddToViewport();

	FInputModeGameOnly InputModeData;

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->SetInputMode(InputModeData);
	PlayerController->SetShowMouseCursor(false);
}

bool UPBDefaultWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(HP_ProgressBar != nullptr)) return false;
	if (!ensure(Stamina_ProgressBar != nullptr)) return false;

	return true;
}
