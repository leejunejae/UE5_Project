// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PBWidgetInterface.h"
#include "PBDefaultWidget.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API UPBDefaultWidget : public UPBWidgetInterface
{
	GENERATED_BODY()
	
public:
	UPBDefaultWidget(const FObjectInitializer& ObjectInitializer);

	// 플레이어 메뉴 기초 설정
	void SetUp();
	virtual bool Initialize();

private:
#pragma region PlayerDefaultUI
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* HP_ProgressBar;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* Stamina_ProgressBar;
#pragma endregion
};
