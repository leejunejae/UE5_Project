// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"
#include "PBWidgetInterface.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API UPBWidgetInterface : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetUp();
	void OffWidget();
};
