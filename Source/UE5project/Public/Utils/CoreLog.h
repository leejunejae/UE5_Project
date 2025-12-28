// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CoreLog.generated.h"


DECLARE_LOG_CATEGORY_EXTERN(Log_Route, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(Log_Check, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(Log_NPCSpawn, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(Log_AI, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(Log_Anim, Log, All);

/**
 * 
 */
UCLASS()
class UE5PROJECT_API UCoreLog : public UObject
{
	GENERATED_BODY()
	
};
