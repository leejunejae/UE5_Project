// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../PBWidgetInterface.h"
#include "PBInteractWidget.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API UPBInteractWidget : public UPBWidgetInterface
{
	GENERATED_BODY()
	
public:
	UPBInteractWidget(const FObjectInitializer& ObjectInitializer);

private:
	UPROPERTY(meta = (BindWidget))
		class UEditableText* InteractText;

	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* InteractKey;
};
