// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AnimBoneTransformLibrary.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EExtractType : uint8
{
	Transform UMETA(DisplayName = "Transform"),
	Vector UMETA(DisplayName = "Vector"),
	Rotator UMETA(DisplayName = "Rotator"),
	Scale UMETA(DisplayName = "Scale"),
};

UCLASS()
class UE5PROJECT_API UAnimBoneTransformLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Trajectory")
		static FTransform GetBoneTransformAtTime(UAnimSequence* AnimSequence, const FName& BoneName, float Time);

	UFUNCTION(BlueprintCallable, Category = "Trajectory")
		static void ExtractAnimBoneTransformToAsset(UAnimSequence* AnimSequence, const FName& BoneName, float FrameRate,const FString& SavePath, const FString& AssetName);
};
